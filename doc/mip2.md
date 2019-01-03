MIP 2: Reserver algorithm bits

Status: Under development



Motivation


Myriadcoin uses bits in the nVersion field to indicate
which PoW algorithm is used.

Currently algo id 6 and 7 defaults to using SHA256d.

For future use of these ids, blocks currently valid
(using id 6 or 7 with SHA256d) should instead not 
be accepted as valid by the network. 



Implementation

Deployment of this rule is through version bit 4.



Reference implementation

https://github.com/myriadteam/myriadcoin/tree/mip2
