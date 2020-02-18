# kvdb

对于一个 db, 最理想的情况是, 数据存储在硬盘上只有一个副本, 已删除的数据不出现在硬盘上. 另外, 数组按桶(Bucket)的形式进行组织, 以便进行哈希和索引.

为了得到更佳的写(和删除)性能, 所以可以允许存储多个副本. 删除标记也是一种副本. 那么, 一个 key 的不同副本会出现在不同的桶中.

### Read

读取 key 的全部副本, 然后取其中的最新版本. 显然, 副本越多, 性能越差. 优化的方向就是减少副本的数量.

### Seek

1. 跳过小于目标的所有 key 之后,
2. 接着跳过所有连续的删除标记.

很显然, 如果连续的删除标记越多, 性能就越差. 考虑这样的一个场景, 有连续的 n 个 key, 它们的非删除副本存储在 Bucket 1 中, 同时, 对应的最新删除标记存储于 Bucket 2 中, 那么, 只能通过一个一个 key 的扫描才能跳过全部的删除标记.

优化方向就是, 当同一个桶中的连续 key 被删除时, 需要尽可能快地将它们从硬盘上真正的清除. 或者能否设计一种数据结构, 能快速地跳过多个 obsoleted key? - range delete?

## Operation

### set(key, val)/del

	appender.set(key, val);

	if(appender.size() > THRESHHOLD){
		range = appender.makeRange();
		ranges.append(range);
	}

### get(key)

	if(val = appender.get(key)){
		return val;
	}
	ranges = select * from ranges where cover(key)
	res = null;
	for(range in ranges){
		if(val = range.get(key)){
			res = max(res, val);
		}
	}

读优化方向: 加快 cover().

* 按 begin 排序, 找 lower_bound, 然后遍历.

### seek(key)/upper_bound

	res = appender.upper_bound(key);
	ranges = select * from ranges where cover(key)
	for(range in ranges){
		res = min(res, range.upper_bound(key));
	}
	if(!ranges && !res){
		range = select * from ranges where range.begin > key order by key limit 1
		return min(res, range.begin);
	}
	return res;

## Range Compaction

减少 Range 的重叠.
