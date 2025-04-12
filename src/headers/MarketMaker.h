#pragma once

/**
 * This agent will provide initial liquidity on the market at specified prices
 * 
 * This really shouldn't be necessary once a server is up and running and people are market making themselves
 *  but it's necessary before anyone actually has any stocks lmao
 * 
 * Each stock should have it's own market maker at first
 */
class MarketMaker {
    
    // should take in an initial value defined in config
    // places limit orders on intial value
    // has a static delta on spread (could eventually be noised or changed based on market conditions)
    // as the price adjusts, maintain trades around the new price
    // do this on a seperate threads?
};