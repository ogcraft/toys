#recordkfn = "../../../asearchdata/makarevich-litsa-fingerpoints.bin"
#samplekfn = "../../../asearchdata/makarevich-litsa-recorded-60-90sec-fingerpoints.bin"
#samplekfn = "../../../asearchdata/makarevich-litsa-recorded-128-134sec-fingerpoints.bin"
#samplekfn = "../../../asearchdata/makarevich-litsa-recorded-9-14sec-fingerpoints.bin"
#samplekfn = "../../../asearchdata/makarevich-litsa-recorded-101-120sec-fingerpoints.bin"

recordkfn = "../../../asearchdata/prometheus-02-eng-dvd-fingerpoints.bin"
samplekfn = "../../../asearchdata/prometeus-record-og-44-225-268sec-fingerpoints.bin"

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
        
function calc_dist(start_pos, record_keys, sample_keys, nsec)
	w = nsec * keys_in_sec
    acc = 0
	for i = 1:w
		d =  count_ones(record_keys[start_pos+i] $ sample_keys[i]) # xor
		acc = acc + d
	end	
	return acc/w
end

function match(record_fn, sample_fn, nsec)
    record_keys = read_keys_from_file(record_fn)
    sample_keys = read_keys_from_file(sample_fn)
    nrecords = length(record_keys)
    nsamples = length(sample_keys)
    @printf "Read keys: %d from: %s\n"  nrecords  record_fn
    @printf "Read keys: %d from: %s\n" nsamples sample_fn
    diffs = fill(33.0, nrecords)
    for i = 1:(nrecords-(nsec+1) * keys_in_sec)
	    diffs[i] = calc_dist(i, record_keys, sample_keys, nsec)
    end
    @printf "Collected %d diffs"  length(diffs)
    return diffs
end

function test1()
    match(recordkfn, samplekfn, 5)
end

