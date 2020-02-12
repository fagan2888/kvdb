# kvdb

## Data Structure

	class Store{
		Range ranges[]; // sorted by range.begin.index
		Index lastIndex;
		Appender appender;
	}

	class Appender{
		Map cache;
		Item items[];
	}

	class Range {
		Key begin;
		Key end;
		Item items[];
	}

	class Key{
		Slice data;
		Index index;
		Action action; // del, set
		int compare();
	}

	class Item{
		Key key;
		Slice val;
	}

## Operation

### get(key)

	if(val = appender.get(key)){
		return val;
	}
	ranges = select * from ranges where contains(key) order by range.begin.index
	foreach(range in ranges){
		if(val = range.get(key)){
			return val;
		}
	}

读优化方向: 加快 contains() 和 order by

* 按 begin 排序, 找 lower_bound, 然后遍历.

### set(key, val)

	appender.set(key, val);

	if(appender.size() > THRESHHOLD){
		range = appender.makeRange();
		ranges.append(range);
	}

### seek(key)/upper_bound




## Range Compaction

减少 Range 的重叠.
