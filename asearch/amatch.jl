#recordkfn = "../../../asearchdata/makarevich-litsa-fingerpoints.bin"
#samplekfn = "../../../asearchdata/makarevich-litsa-recorded-60-90sec-fingerpoints.bin"
#samplekfn = "../../../asearchdata/makarevich-litsa-recorded-9-14sec-fingerpoints.bin"
#samplekfn = "../../../asearchdata/makarevich-litsa-recorded-101-120sec-fingerpoints.bin"

#recordkfn = "../../../asearchdata/prometheus-02-eng-dvd-fingerpoints.bin"
#samplekfn = "../../../asearchdata/prometeus-record-og-44-225-268sec-fingerpoints.bin"

recordkfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-60-90sec-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-30-90sec-amp-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-06-30sec-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-14-40sec-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-170sec-amp-fingerpoints.bin"
samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-30-90sec-amp-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-38-60sec-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-60-90sec-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-69sec-amp-fingerpoints.bin"
#samplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-69sec-fingerpoints.bin"

keys_in_sec = 86
sec_per_sample =  0.01164 

function read_keys_from_file(filename::String)
    io = open(filename)
    magic = read(io, Int32)
    n = read(io, Int32)
    a = Array(Uint32, n)
    i = 0        
    while !eof(io)
        i += 1
        a[i] = read(io, Uint32)
    end
    return a
end

function calc_dist(start_pos, record_keys, sample_keys, nsec, shift_sec)
	w = nsec * keys_in_sec
    shift = shift_sec * keys_in_sec
    acc = 0
	for i = 1:w
		d =  count_ones(record_keys[start_pos+i] $ sample_keys[i+shift]) # xor
		acc = acc + d
	end	
	return int(acc/w)
end

function match(record_fn, sample_fn, nsec)
    record_keys = read_keys_from_file(record_fn)
    sample_keys = read_keys_from_file(sample_fn)
    nrecords = length(record_keys)
    nsamples = length(sample_keys)
    @printf "Read keys: %d from: %s\n"  nrecords  record_fn
    @printf "Read keys: %d from: %s\n" nsamples sample_fn
    nrecords = 10000
    diffs = fill(33, nrecords)
    diffs1 = fill(33, nrecords)
    for i = 1:(nrecords-(nsec+1) * keys_in_sec)
	    diffs[i] = calc_dist(i, record_keys, sample_keys, nsec, 0)
	    diffs1[i] = calc_dist(i, record_keys, sample_keys, nsec, 5)
    end
    @printf "Collected %d diffs\n"  length(diffs)
    m,index=findmin(diffs)
    m1,index1=findmin(diffs1)
    @printf "Found min: %d index: %d sec: %f\n\n" m index index*sec_per_sample
    @printf "Found min: %d index: %d sec: %f\n\n" m1 index1 index1*sec_per_sample
    @printf "Diff t1-t2: %f\n\n" (index1-index)*sec_per_sample
    return diffs, diffs1
end

function test1()
    match(recordkfn, samplekfn, 3)
end

