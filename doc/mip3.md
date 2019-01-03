MIP 3: Longblocks

Status: Under development


Motivation


With the emergence of layer 2 solutions, the need for fast block confirmations 
is lessened. Additionally, chainindex bloat is a significant issue to the 
long-term health of Myriadcoin.

This proposal implements a gradual increase in block time from 1 to 8 minutes 
using multiple intervals of block time adjustments.

Additional reasons for this change:

* Capturing additional improvements from upstream indexing and development.
* Individual block security is improved due to the higher difficulty.
* Orphan count is expected to decrease.


Implementation

In three phases this proposal plans an increase in block time while keeping 
the reward amount essentially the same. In practice, this requires raising the 
block reward at a scale proportional to the block time increase. Myriadcoin 
would not see another block halving until the 6th block halving. Then block 
halvings continue at the original schedule of ~2yrs.

Below is an approximate table to show how this progresses.

| Subsidy Halving Interval |  Reward | Block Time | Final Blk | Aprox. Date |
| ------------------------ | ------- | ---------- | --------- | ----------- |
|    1 (967680 blocks)     |  1000.0 |  0.5 min   |  967680   |   Feb 2015  |
|    2 (967680 blocks)     |  500.0  |   ~1 min   |  1935360  |   Jan 2017  |
|    3 (967680 blocks)     |  250.0  |    1 min   |  2903040  |  ~May 2019  |
|    4 **(483840 blocks)** |  250.0  | **2 min**  |  3386880  |  ~May 2021  |
|    5 **(241920 blocks)** |  250.0  | **4 min**  |  3628800  |  ~May 2023  |
|    6 **(120960 blocks)** |  250.0  | **8 min**  |  3749760  |  ~May 2025  |
|    7 (120960 blocks)     |  125.0  |    8 min   |  3870720  |  ~May 2027  |
|    8 (120960 blocks)     |  62.5   |    8 min   |  3991680  |  ~May 2029  |
|  ... (120960 blocks)     |  ...    |    8 min   |  ...      |  ...        |

Halvings would then continue at 120960 block intervals.

It should be noted that MAX_MONEY is not changed and emission rate vs. clock 
time should not be affected.

Deployment of this rule is through version bit 5.



Reference implementation

https://github.com/cryptapus/bitcoin/tree/myriadcoin.master.lngblcks/doc/mip3.md
