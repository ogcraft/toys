////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>         // std::string
#include <bitset>         // std::bitset

#include <sndfile.h>
#include "armadillo"
#include "utils.h"
#include "sigproc.h"
//#include "fplib/FingerprintExtractor.h"

using namespace arma;




//const char* chimes_wav = "/Users/olegg/git/toys/asearch/chimes.wav";
//const char* prom_dvd = "/Users/olegg/asearchdata/prometheus-02-eng-dvd.wav";
//const char* prom_tst = "/Users/olegg/asearchdata/prometheus-ts.wav";
//const char* dq = "/Users/olegg/asearchdata/dancingqueen.wav";

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

        SF_FORMAT_INFO  format_info;

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
#if 0
for (;;)
      {
         // read some data from the file
         size_t sz = read_some_source( pPCMBuffer );
         if ( sz == 0 )
         {
            cerr << "ERROR: Insufficient input data!" << endl;
            exit(1); // goodbye!
         }

         // Process to create the fingerprint. If process returns true
         // it means he's happy with what he has.
         if ( fextr.process( pPCMBuffer, sz
                 /*, bool: set it to true if it's EOF */ ) )
            break;
      }

      // get the fingerprint data
      pair<const char*, size_t> fpData = fextr.getFingerprint();

#endif

int test1(const char* wavfn) 
{
    
  vector<Filter> filters;
  printf ("Reading %s...\n", wavfn);
  unsigned int nsamples = 0, freq = 0;
  float * samples = wavread(wavfn, &nsamples, &freq);

  if (!samples) {
    printf("Error reading wave file.\n");
    return 0;
  }

  unsigned int nbits;
  unsigned int * bits = wav2bits(filters, samples, nsamples, freq, &nbits);

  free(samples);

  printf("Writing %d keys...\n", nbits);

  //writebits(bits, nbits, outfn);
  
  for (unsigned int i = 0; i < 1000 /* nbits */; i++) {
	  std::bitset<32> b(bits[i]);
	  std::cout << b << std::endl;
  }

  free(bits);

  return 0;  
  
}


int main(int argc, char* argv[])
{
  int ret = 0;
  
  std::cout << "Armadillo version: " << arma_version::as_string() << std::endl;
  char  sndflile_ver [128] ;
  sf_command (NULL, SFC_GET_LIB_VERSION, sndflile_ver, sizeof (sndflile_ver)) ;
  std::cout << "Sndfile version: " << sndflile_ver << std::endl << std::endl;
  //FingerprintExtractor fextr = FingerprintExtractor;
  //std::cout << "fplib version: " << fingerprint::FingerprintExtractor::getVersion() << std::endl;


  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << " wav_file_name" << std::endl;
    exit(1);
  }

  //size_t min_duration_ms = fingerprint::FingerprintExtractor::getMinimumDurationMs();
  //std::cout << "Minimal duartion if audio " << min_duration_ms << " msec";
  const char* wavfile = argv[1]; 
  std::cout << "Running " << argv[0] << " with '" << argv[1] << "'" << std::endl << std::endl;
  
  dump_snd_file_info(wavfile);

  vector<Filter> filters;// = readFilters(descrfn);

  SF_INFO sndinfo;
  SNDFILE *sndfile = sf_open(wavfile, SFM_READ, &sndinfo);
  if (sndfile == NULL) {
    std::cout << "Error reading source file '" << wavfile << "': " << sf_strerror(sndfile) << std::endl;
    return 1;
  }

  int nchannels = sndinfo.channels;





  int err = sf_close(sndfile);
  if(err)
  {
    std::cout << "Close Error: " << sf_strerror(sndfile) << std::endl;
    return 1;
  }

  test1(wavfile);
  
  return ret;
}

#if 0
3. Running the Music Identification Server
------------------------------------------

"mrserver boostextdescr.txt db.fdb db.kdb emparams.bin [port]"

26752 -4.37515e-07 0.260836
23871 -2.44615e-05 0.263986
26777 -3.69244e-08 0.267763
4635 -1.13672e-05 0.269428
2937 5.28804e-09 0.271896
27405 -0.000126494 0.272362
10782 4.27478e-08 0.272609
21033 -6.7912e-07 0.276099
27117 8.07178e-06 0.277762
27072 2.46044e-05 0.27883
24228 4.11255e-07 0.281743
23838 0.000228396 0.284479
17165 -1.19495e-07 0.286304
25263 0.000398279 0.287066
20721 7.15095e-07 0.288913
8502 -2.78361e-07 0.290424
17175 -1.08429e-08 0.292219
17811 -3.29527e-08 0.292554
27495 -4.47575e-07 0.290119
23538 -3.04273e-09 0.294539
8205 4.02691e-07 0.293525
12177 1.16873e-06 0.293832
27051 -0.000902544 0.296453
27111 -2.38425e-05 0.297428
21779 -1.0669e-07 0.297302
14817 -9.52849e-09 0.299
27087 1.22163e-05 0.296502
27081 -2.8758e-09 0.300112
20394 1.28237e-06 0.298693
28209 0.000624447 0.29812
23533 -2.19406e-06 0.299773
23865 -1.28037e-08 0.300777

vector<Filter> readFilters(char * fn) {
  FILE * f = fopen(fn, "rb");

  if (!f) {
    printf("Error reading descriptor file.\n");
    exit(1);
  }

  unsigned int ftid;
  float thresh;
  float weight;

  vector<Filter> filters;

  while (fscanf(f, "%d %f %f", &ftid, &thresh, &weight) == 3) {
    filters.push_back(Filter(ftid, thresh, weight));
    /*
    Filter ft = filters[filters.size() - 1];
    printf("ID: %d   wt: %d  wb: %d fb: %d type: %d\n",
           ft.id, ft.wt, ft.wb, ft.first_band, ft.filter_type);
    */
  }

  printf("Read %d filters.\n", filters.size());

  fclose(f);

  return filters;
}
#endif


////////////////////////////////////////
#if 0
int main(int argc, char* argv[])
{
  int ret = 0;
  
  std::cout << "Armadillo version: " << arma_version::as_string() << std::endl;
  char  sndflile_ver [128] ;
  sf_command (NULL, SFC_GET_LIB_VERSION, sndflile_ver, sizeof (sndflile_ver)) ;
  std::cout << "Sndfile version: " << sndflile_ver << std::endl << std::endl;

  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << " wav_file_name" << std::endl;
    exit(1);
  }

  const char* wavfile = argv[1]; 
  std::cout << "Running " << argv[0] << " with '" << argv[1] << "'" << std::endl << std::endl;
  
  dump_snd_file_info(wavfile);

  SF_INFO sndinfo;
  SNDFILE *sndfile = sf_open(wavfile, SFM_READ, &sndinfo);
  if (sndfile == NULL) {
    std::cout << "Error reading source file '" << wavfile << "': " << sf_strerror(sndfile) << std::endl;
    return 1;
  }

  int nchannels = sndinfo.channels;
#if 0
  double buffer [1024] ;
  sf_count_t count = sfx_mix_mono_read_double (sndfile, buffer, ARRAY_LEN (buffer));
  std::cout << count << " Frames read and mixed to mono " << std::endl;
  colvec v(buffer, count);

  //std::cout << "v: " << v << std::endl;
  v.save("asearch.dat", raw_ascii);
#endif
  //sf_seek(sndfile, 50000, SEEK_SET);
  int nframes = 441000;
  mat D(2, nframes);  
  sf_count_t read_frames = load_stereo_file_into_matrix(sndfile, nframes, D );

  //std::cout << D << std::endl;
  
  std::cout << "D.n_cols: " << D.n_cols << " D.n_rows: " << D.n_rows << std::endl;
  mat M = mean(D);
  std::cout << "Mean calculated " << std::endl;
  //std::cout << "Mono: " << M.row(0) << std::endl;

  std::cout << "M.n_cols: " << M.n_cols << " M.n_rows: " << M.n_rows << " read_frames: " << read_frames << std::endl;
  
  mat Spectr;
  int ntffs = 512;
  
  spectrogram(M, ntffs, ntffs/2, Spectr);

  std::cout << "Spectr len: " << Spectr.n_elem << " cols: " << Spectr.n_cols << " rows: " << Spectr.n_rows 
  << " sizeof: " << sizeof(Spectr) << std::endl;
  
  Spectr.save("asearch.dat", raw_ascii);

  int err = sf_close(sndfile);
  if(err)
  {
    std::cout << "Close Error: " << sf_strerror(sndfile) << std::endl;
    return 1;
  }

  return ret;
}
#endif
