# IP-Datagram-Composer-Decomposer
Files to create an IP datagram from given data, and then to extract that information from that very datagram. - Vighnesh Souda

Instructions:
0. Edit composer.c to reflect your given data in the u_char data[] variable.
1. Compile composer.c, and execute the output file. A text file called "ip_datagram.txt" should have been created. This file contains all the information for the IP datagram we are going to extract.
2. Compile decomposer.c, and execute the output file. The command window should display all characteristics about the datagram.

```
$ gcc composer.c -o out; ./out
$ gcc decomposer.c -o out2; ./out2

```
### Sample Output
![Alt Text](https://puu.sh/we6ul/f0e99693fc.png)
