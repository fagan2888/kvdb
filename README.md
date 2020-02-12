# kvdb

## Data Structure

将 del 和 set 分开存储, 避免 seek 的时候, 需要跳过大量 del 的情况(http://www.ideawu.net/blog/archives/1083.html).

	class Database{
		Store sets;
		Store dels;
		Appender appender;
	}

	class Appender{
		Map cache;
		Index lastIndex;
		Item items[];
	}

	class Store{
		Range ranges[]; // sorted by range.begin.index
	}

	class Range {
		Key begin;
		Key end;
		Item items[];
	}

	class Key{
		Slice data;
		Index index;
		int compare();
	}

	class Item{
		Key key;
		Slice val;
	}

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
	ranges.order_by(begin.index).desc();
	for(range in ranges){
		if(val = range.get(key)){
			return val;
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
