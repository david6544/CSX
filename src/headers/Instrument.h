#include<cinttypes>
#include<functional>

namespace instrument {
    
    struct Ticker {
        uint16_t ticker;

        // Add comparison operators for map/set compatibility
        bool operator==(const Ticker& other) const {
            return ticker == other.ticker;
        }
        
        bool operator<(const Ticker& other) const {
            return ticker < other.ticker;
        }
    };

    // format of name will be
    
     
    struct TickerHash {
        size_t operator()(const instrument::Ticker& a) const {
            return std::hash<uint16_t>()(a.ticker); 
        }
    };
    
    struct Order {
        uint64_t time;   
        uint32_t price;
        uint32_t quantity;
    };

    static constexpr bool isOrderEqual(const Order l, const Order r) {
        if (l.time == r.time && l.price == r.price && l.quantity == r.quantity) {
            return true;
        }
        
        return false;
    }

    enum class OrderType {
        Ask = 0,
        Bid = 1,
    };

        
    // Price/time order match for bids
    struct BidComparator {
        bool operator()(const Order l, const Order r) const {
            if (l.price != r.price) {
                return l.price < r.price; // Greater Price first
            }
            return l.time > r.time; 
        }
    };


    // Price/time order match for asks
    struct AskComparator {
        bool operator()(const Order l, const Order r) const {
            if (l.price != r.price) {
                return l.price > r.price; // Lesser price first
            }
            return l.time > r.time; 
        }
    };

}
