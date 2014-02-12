////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include <sndfile.h>
#include "armadillo"
#include "utils.h"
#include "fplib/FingerprintExtractor.h"

using namespace arma;

#define ARRAY_LEN(x)  ((int) (sizeof (x) / sizeof (x [0])))
#define MAX(x,y)    ((x) > (y) ? (x) : (y))
#define MIN(x,y)    ((x) < (y) ? (x) : (y))

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

int main(int argc, char* argv[])
{
  int ret = 0;
  
  std::cout << "Armadillo version: " << arma_version::as_string() << std::endl;
  char  sndflile_ver [128] ;
  sf_command (NULL, SFC_GET_LIB_VERSION, sndflile_ver, sizeof (sndflile_ver)) ;
  std::cout << "Sndfile version: " << sndflile_ver << std::endl << std::endl;
  //FingerprintExtractor fextr = FingerprintExtractor;
  std::cout << "fplib version: " << fingerprint::FingerprintExtractor::getVersion() << std::endl;


  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << " wav_file_name" << std::endl;
    exit(1);
  }

  size_t min_duration_ms = fingerprint::FingerprintExtractor::getMinimumDurationMs();
  std::cout << "Minimal duartion if audio " << min_duration_ms << " msec";
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





  int err = sf_close(sndfile);
  if(err)
  {
    std::cout << "Close Error: " << sf_strerror(sndfile) << std::endl;
    return 1;
  }

  return ret;
}
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
