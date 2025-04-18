#pragma once
#include<cinttypes>
#include<random>
#include<chrono>
#include"Market.h"

/**
 * This agent will provide initial liquidity on the market at specified prices
 * 
 * This really shouldn't be necessary once a server is up and running and people are market making themselves
 *  but it's necessary before anyone actually has any stocks lmao
 * 
 * Each stock should have it's own market maker at first
 */
class MarketMaker {
    public:
    // should take in an initial value defined in config
    // places limit orders on intial value
    // has a static delta on spread (could eventually be noised or changed based on market conditions)
    // as the price adjusts, maintain trades around the new price
    // do this on a seperate threads?

    void addLiquidity(Market& market, instrument::ticker ticker);
    void setPrice(uint32_t price);


    static constexpr double spread = 0.15;
    static constexpr double stddev = 0.005;

    private:
    uint16_t price;

};