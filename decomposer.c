#include <stdio.h>
#include "ip.h"
#include <string.h>
#include <stdlib.h>

int findLengthOfPacket(FILE *file){
  int headerBegin[3];
  for(int i = 0;i<=3;i++){
    fscanf(file,"%d.",&headerBegin[i]);
  }
  return ((headerBegin[2]<<8)|(headerBegin[3]));
}

void checkChecksum(int arr[],int headerLength){
  int leastSignifDigitSum = 0;
  int mostSignifDigitSum = 0;
  int carry = 0;
  for(int i=0;i<headerLength;i++){
    if (i%2)
      leastSignifDigitSum += arr[i];
    else
      mostSignifDigitSum += arr[i];
  }
  carry = (leastSignifDigitSum/256);
  leastSignifDigitSum = leastSignifDigitSum-256*carry;
  mostSignifDigitSum+=carry;
  carry = (mostSignifDigitSum/256);
  mostSignifDigitSum = mostSignifDigitSum-256*carry;
  leastSignifDigitSum+=carry;
  u_char check = ~((mostSignifDigitSum<<8)|(leastSignifDigitSum));
  if (!check)
    printf("Checksum computes no error. Datagram is good.\n");
}

int main(){
  FILE *packet = fopen("ip_datagram.txt","r");
  int totalLength = findLengthOfPacket(packet);
  int allData[totalLength];
  FILE *packet2 = fopen("ip_datagram.txt","r");
  for(int i = 0;i<totalLength;i++){
    fscanf(packet2,"%d.",&allData[i]);
    //printf("%d\n",allData[i]);
  }
  fclose(packet);
  fclose(packet2);
  int vers = ((allData[0]&240) >> 4); //mask is 240 = 11110000
  int hlen = ((allData[0]&15)); //mask is 15 = 00001111
  int tos = allData[1];
  totalLength = ((allData[2]<<8)|(allData[3]));
  int identif = ((allData[4]<<8)|(allData[5]));
  int fragoff =  ((allData[6]<<8)|(allData[7]));
  int ttl = allData[8];
  int proto = allData[9];
  char cksum[]="\n",srcip[]="\n",destip[]="\n";
  sprintf(cksum,"%d.%d",allData[10],allData[11]);
  sprintf(srcip,"%d.%d.%d.%d",allData[12],allData[13],allData[14],allData[15]);
  sprintf(destip,"%d.%d.%d.%d",allData[16],allData[17],allData[18],allData[19]);
  char data[] = "\n";

  for(int i=hlen*4;i<totalLength;i++){
    char c = allData[i];
    data[i-hlen*4] = c;
  }



  printf("IP Version: %d\n",vers);
  printf("Header Length: %dx4\n",hlen);
  printf("Type of service: %d\n",tos);
  printf("Total Length of Datagram: %d\n",totalLength);
  printf("Datagram ID: %d\n",identif);
  printf("Fragment Offset: %d\n",fragoff);
  printf("Time to live: %d\n",ttl);
  printf("IP Protocol: %d\n",proto);
  printf("Checksum: %s\n", cksum);
  printf("Source IP: %s\n", srcip);
  printf("Dest IP: %s\n", destip);
  printf("Data: '%s'\n",data);
  checkChecksum(allData,hlen*4);

  return(0);
}
