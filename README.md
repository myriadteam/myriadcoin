Myriadcoin Core integration/staging tree
=====================================

[![Build Status](https://travis-ci.com/myriadteam/myriadcoin.svg?branch=master)](https://travis-ci.com/myriadteam/myriadcoin)

http://www.myriadcoin.org

What is Myriadcoin?
----------------

Myriadcoin is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. Myriadcoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Myriadcoin Core is the name of open source
software which enables the use of this currency.

For more information, as well as an immediately usable, binary version of
the Myriadcoin Core software, see http://www.myriadcoin.org, or read the
[original Bitcoin whitepaper](https://bitcoincore.org/bitcoin.pdf).

Myriadcoin is distinct from Bitcoin in that it pioneered the use of multiple independent 
PoW algorithms on the same chain.

License
-------

Myriadcoin is a fork of Bitcoin Core and inherits the terms of the MIT license. See 
[COPYING](COPYING) for more information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is occasionally built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/myriadteam/myriadcoin/tags) are created
regularly to indicate new official, stable release versions of Myriadcoin Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

The developer [subreddit](https://www.reddit.com/r/myriadcoin)
should be used to discuss complicated or controversial changes before working
on a patch set.

Developer IRC can be found on Freenode at ##myriadcoin.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Myriadcoin Core's Transifex page](https://www.transifex.com/projects/p/myriadcoin/).

Translations are periodically pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.

Translators should also visit the [myriadcoin subreddit](https://www.reddit.com/r/myriadcoin).
