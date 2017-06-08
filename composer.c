#include <stdio.h>
#include "ip.h"
#include <string.h>

#define headerLength (IP_MINHLEN*4) //IP_MINHLEN's value is defined in the header
//IP_MINHLEN is length of header in terms of 4 bytes

#define verlen ((IP_VERSION<<4) + (headerLength/4))
//IP_VERSION's value is defined in the header file, and we make it the 4 most signif. bits here
//since headerLength is <5 bits, we simply add it

u_char data[] = "One small step for man, but one big leap for mankind.\0";
//this is the data we will put in our datagram

#define lengthOfData strlen(data) //length of data in bytes
#define serviceType 0  //normal service type
#define totalLength (headerLength + lengthOfData)
#define ipIdentification 3251  //random number for the identification
#define fragOffset 0 //fragmentation offset in terms of 8 bytes
#define timeToLive 8 //time to live value used is given in header file
#define ipProtocol IPT_ICMP //ip protocol value used is given in header file
#define sourceIP ((145<<24)|(23<<16)|(78<<8)|197)
#define destinationIP ((201<<24)|(44<<16)|(159<<8)|23)

u_char arrayOfBytes[headerLength];

int systemIsLittleEndian(){
  short numberToUse = 256; //0x100
  char value = * (char*) &numberToUse;
  if(value==0)
    return(1); //if it is little endian, return 1
  return(0); //else return 0
}

void reverse(u_char array[], int start, int end){
  int temp;
  while (start < end){
    temp = array[start];
    array[start] = array[end];
    array[end] = temp;
    start++;
    end--;
  }
}

void putDataInArray(u_char array[], struct ip* addressDatagram){
  for(int i=totalLength+12;i>=headerLength+12;i--){
    array[i-12] = *((u_char*)(addressDatagram)+(i));
  }
}

short calculateChecksum(struct ip* addressDatagram){
  int j=1;
  for(int i=1;i<=headerLength+8;i++){
    if(i==13|i==21) //since my system is x64 bit, unsigned long (IPaddr) is 8 bytes
      i+=4; //therefore, I skip the unneeded 4 least signif. bits to get the ip addresses
    arrayOfBytes[j-1] = *((u_char*)(addressDatagram)+(i-1));
    j++;
  }

  if(systemIsLittleEndian){
    reverse(arrayOfBytes,2,3);
    reverse(arrayOfBytes,4,5);
    reverse(arrayOfBytes,6,7);
    reverse(arrayOfBytes,10,11);
    reverse(arrayOfBytes,12,15);
    reverse(arrayOfBytes,16,19);
  }

  int leastSignifDigitSum = 0;
  int mostSignifDigitSum = 0;
  int carry = 0;


  for(int i=0;i<headerLength;i++){
    if (i%2)
      leastSignifDigitSum += arrayOfBytes[i];
    else
      mostSignifDigitSum += arrayOfBytes[i];
  }
  carry = (leastSignifDigitSum/256);
  leastSignifDigitSum = leastSignifDigitSum-256*carry;
  mostSignifDigitSum+=carry;
  carry = (mostSignifDigitSum/256);
  mostSignifDigitSum = mostSignifDigitSum-256*carry;
  leastSignifDigitSum+=carry;
  int checksum = ~((mostSignifDigitSum<<8)|(leastSignifDigitSum));
  arrayOfBytes[10] = ~(mostSignifDigitSum);
  arrayOfBytes[11] = ~(leastSignifDigitSum);
  return(checksum);
}

int main(){
  struct ip datagram;
  struct ip *addressDatagram = &datagram;
  addressDatagram->ip_verlen = verlen;
  addressDatagram->ip_tos = serviceType;
  addressDatagram->ip_len = totalLength;
  addressDatagram->ip_id = ipIdentification;
  addressDatagram->ip_fragoff = fragOffset;
  addressDatagram->ip_ttl = timeToLive;
  addressDatagram->ip_proto = ipProtocol;
  addressDatagram->ip_cksum = 0;
  addressDatagram->ip_src = sourceIP;
  addressDatagram->ip_dst = destinationIP;
  addressDatagram->ip_cksum = calculateChecksum(addressDatagram);
  //addressDatagram->ip_data[1] = data;
  memcpy(addressDatagram->ip_data,data,lengthOfData);
  putDataInArray(arrayOfBytes,addressDatagram);
  for(int i = 0;i<totalLength;i++){
    printf("%u ",arrayOfBytes[i]);
  }
  FILE *fp;
  fp = fopen("ip_datagram.txt","wb");
  for(int i = 0;i<totalLength;i++){
    fprintf(fp,"%u.",arrayOfBytes[i]);
  }
  fclose(fp);


  return 0;
}

