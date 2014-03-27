#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/circular_buffer.hpp>

#define CONV16BIT 32768
#define CONVMYFLT (1./32768.)
#define GRAIN 4

typedef struct _circular_buffer {
	char *buffer;
	int  wp;
	int rp;
	int size;
} circular_buffer;

circular_buffer* create_circular_buffer(int bytes);
int checkspace_circular_buffer(circular_buffer *p, int writeCheck);
int read_circular_buffer_bytes(circular_buffer *p, char *out, int bytes);
int write_circular_buffer_bytes(circular_buffer *p, const char *in, int bytes);
void free_circular_buffer (circular_buffer *p);


#if 0
  if((p->outrb = create_circular_buffer(p->outBufSamples*sizeof(short)*4)) == NULL) {
      android_CloseAudioDevice(p);
      return NULL; 
  }
 if((p->inrb = create_circular_buffer(p->outBufSamples*sizeof(short)*4)) == NULL) {
      android_CloseAudioDevice(p);
      return NULL; 
 }

  free_circular_buffer(p->inrb);
  free_circular_buffer(p->outrb);

// returns timestamp of the processed stream
double android_GetTimestamp(OPENSL_STREAM *p){
  return p->time;
}


// this callback handler is called every time a buffer finishes recording
void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
  OPENSL_STREAM *p = (OPENSL_STREAM *) context;
  int bytes = p->inBufSamples*sizeof(short);
  write_circular_buffer_bytes(p->inrb, (char *) p->recBuffer,bytes);
  (*p->recorderBufferQueue)->Enqueue(p->recorderBufferQueue,p->recBuffer,bytes);
}
 
// gets a buffer of size samples from the device
int android_AudioIn(OPENSL_STREAM *p,float *buffer,int size){
  short *inBuffer;
  int i, bytes = size*sizeof(short);
  if(p == NULL ||  p->inBufSamples ==  0) return 0;
  bytes = read_circular_buffer_bytes(p->inrb,(char *)p->inputBuffer,bytes);
  size = bytes/sizeof(short);
  for(i=0; i < size; i++){
    buffer[i] = (float) p->inputBuffer[i]*CONVMYFLT;
  }
  if(p->outchannels == 0) p->time += (double) size/(p->sr*p->inchannels);
  return size;
}

// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
  OPENSL_STREAM *p = (OPENSL_STREAM *) context;
  int bytes = p->outBufSamples*sizeof(short);
  read_circular_buffer_bytes(p->outrb, (char *) p->playBuffer,bytes);
  (*p->bqPlayerBufferQueue)->Enqueue(p->bqPlayerBufferQueue,p->playBuffer,bytes);
}

// puts a buffer of size samples to the device
int android_AudioOut(OPENSL_STREAM *p, float *buffer,int size){

  short *outBuffer, *inBuffer;
  int i, bytes = size*sizeof(short);
  if(p == NULL  ||  p->outBufSamples ==  0)  return 0;
  for(i=0; i < size; i++){
    p->outputBuffer[i] = (short) (buffer[i]*CONV16BIT);
  }
  bytes = write_circular_buffer_bytes(p->outrb, (char *) p->outputBuffer,bytes);
  p->time += (double) size/(p->sr*p->outchannels);
  return bytes/sizeof(short);
}
#endif

circular_buffer* create_circular_buffer(int bytes){
  circular_buffer *p;
  if ((p = (circular_buffer*)calloc(1, sizeof(circular_buffer))) == NULL) {
    return NULL;
  }
  p->size = bytes;
  p->wp = p->rp = 0;
   
  if ((p->buffer = (char*)calloc(bytes, sizeof(char))) == NULL) {
    free (p);
    return NULL;
  }
  return p;
}

int checkspace_circular_buffer(circular_buffer *p, int writeCheck){
  int wp = p->wp, rp = p->rp, size = p->size;
  if(writeCheck){
    if (wp > rp) return rp - wp + size - 1;
    else if (wp < rp) return rp - wp - 1;
    else return size - 1;
  }
  else {
    if (wp > rp) return wp - rp;
    else if (wp < rp) return wp - rp + size;
    else return 0;
  }	
}

int read_circular_buffer_bytes(circular_buffer *p, char *out, int bytes){
  int remaining;
  int bytesread, size = p->size;
  int i=0, rp = p->rp;
  char *buffer = p->buffer;
  if ((remaining = checkspace_circular_buffer(p, 0)) == 0) {
    return 0;
  }
  bytesread = bytes > remaining ? remaining : bytes;
  for(i=0; i < bytesread; i++){
    out[i] = buffer[rp++];
    if(rp == size) rp = 0;
  }
  p->rp = rp;
  return bytesread;
}

int write_circular_buffer_bytes(circular_buffer *p, const char *in, int bytes){
  int remaining;
  int byteswrite, size = p->size;
  int i=0, wp = p->wp;
  char *buffer = p->buffer;
  if ((remaining = checkspace_circular_buffer(p, 1)) == 0) {
    return 0;
  }
  byteswrite = bytes > remaining ? remaining : bytes;
  for(i=0; i < byteswrite; i++){
    buffer[wp++] = in[i];
    if(wp == size) wp = 0;
  }
  p->wp = wp;
  return byteswrite;
}

void
free_circular_buffer (circular_buffer *p){
  if(p == NULL) return;
  free(p->buffer);
  free(p);
}

using namespace std;

int main(int argc, char* argv[])
{
	const char* data= "0123456789";
	char out[1024]={0};
	printf("%s running...\n", argv[0]);
	cout << "This thread: " << boost::this_thread::get_id() << endl; 
	const int BUFSIZE=5;
	circular_buffer *p = create_circular_buffer(BUFSIZE);
	assert(p);
	int nw = write_circular_buffer_bytes(p, data, 10);
	cout << "Written bytes: " << nw << " data: " << data << endl;

	int nr = read_circular_buffer_bytes(p, out, 10);
	cout << "Read bytes: " << nr << "  out: " << out << endl;
	cout << "----------------------------------------" << endl;
	boost::circular_buffer<char> cb(5);
	cb.insert(cb.end(), data, data + 7);

	cout << "cb sz: " << cb.size() << " capacity: " << cb.capacity() << endl;
	memcpy(out, cb.linearize(), cb.size());
	cout << "Total out: [" << out << "]" << endl;
	memset(out, 0, 1024);
	
	copy(cb.begin(),cb.end(),out);
	cout << "Total 2 out: [" << out << "]" << endl;
	memset(out, 0, 1024);

	copy(cb.end()-2 ,cb.end(),out);
	cout << "Last 2 out: [" << out << "]" << endl;

	free(p);
	return 0;
}
