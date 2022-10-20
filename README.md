# tradesys
Simple market-data feed and strategy trading system to learn C, network programming, and one-pass algorithms.

## Usage
Build the `feed` and `strategy` binaries with `cmake` followed by `make`, then run the two binaries as separate processes.
`feed` sends quotes over UDP, which are received by `strategy` and processed by one-pass algorithms to estimate market liquidity and volatility. The `QUOTE_FILEPATH` environment variable must be set, which should point to the `cfg/quote.csv` configuration file. Each line in this file represents a single market and some bounds for generating quotes.
