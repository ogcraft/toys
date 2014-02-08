////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include <sndfile.h>
#include "armadillo"

using namespace arma;

const char* chimes_wav = "/Users/olegg/git/toys/asearch/chimes.wav";
const char* prom_dvd = "/Users/olegg/asearchdata/prometheus-02-eng-dvd.wav";
const char* prom_tst = "/Users/olegg/asearchdata/prometheus-ts.wav";
const char* dq = "/Users/olegg/asearchdata/dancingqueen.wav";

void dump_sf_info(const SF_INFO& sf_info, const char* msg = "")
{
	  	typedef struct
      	{   
      		sf_count_t  frames ;     /* Used to be called samples. */
           	int         samplerate ;
           	int         channels ;
           	int         format ;
           	int         sections ;
           	int         seekable ;
       	} SF_INFO ;

       	SF_FORMAT_INFO	format_info;

       	format_info.format = sf_info.format ;
        sf_command (NULL, SFC_GET_FORMAT_INFO, &format_info, sizeof (format_info)) ;
        
       	std::cout << "SF_INFO " << msg 
       	<< " frames:" << sf_info.frames << " samplerate:" << sf_info.samplerate 
       	<< " channels:" << sf_info.channels 
       	<< " format: 0x" << std::hex << sf_info.format << std::dec << " '" << format_info.name << "'"
       	<< " sections:" << sf_info.sections << " seekable:" << sf_info.seekable 
       	<< std::endl;  
}

void dump_snd_file_info(const char* fn)
{

	SF_INFO sf_info;
	SNDFILE* sf = sf_open(fn, SFM_READ, &sf_info) ;
	if(!sf)
	{
		std::cout << "dump_snd_file_info(): Open " << fn << " Error: " << sf_strerror(sf) << std::endl;
		return;
	}
	dump_sf_info(sf_info, fn);
	int err = sf_close(sf);
	if(err)
	{
		std::cout << "dump_snd_file_info(): Close Error: " << err << std::endl;
	}
}

int main(int argc, char* argv[])
{
	int ret = 0;
	std::cout << "Running " << argv[0] << std::endl;
	std::cout << "Armadillo version: " << arma_version::as_string() << std::endl;
	char  sndflile_ver [128] ;
    sf_command (NULL, SFC_GET_LIB_VERSION, sndflile_ver, sizeof (sndflile_ver)) ;
    std::cout << "Sndfile version: " << sndflile_ver << std::endl;

    //dump_snd_file_info(chimes_wav);
    //dump_snd_file_info(prom_dvd);
    //dump_snd_file_info(prom_tst);
	dump_snd_file_info(dq);



	return ret;
}