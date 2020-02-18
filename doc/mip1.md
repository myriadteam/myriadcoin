MIP 1: Algorithm change - Equihash

Status: Under development




Motivation


Myriad strives to have a balance of ASIC, GPU and CPU algorithms.
At the moment the balance has shifted to ASIC algorithms.
The Equihash algorithm is proposed as new algorithm to shift the balance
back towards GPU mining.




Implementation description


Equihash is added as the seventh algorithm with ID 6.

The Solution variable is conditionally streamed for Equihash blocks
to include it in the hash.

It is assumed the algorithm change will be introduced through
the version bits consensus mechanism.

Current reference implementation does not yet address introduction
of the new algorithm nor retirement of an existing algorithm.




Reference implementation


https://github.com/myriadteam/myriadcoin/tree/mip1
