#include<cinttypes>

namespace instrument {
    
    struct Ticker {
        uint16_t ticker;
    };

    // format of name will be

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
    struct BidOrder {
        bool operator()(const Order l, const Order r) const {
            if (l.price != r.price) {
                return l.price < r.price;
            }
            return l.time > r.time; 
        }
    };


    // Price  r
    struct AskOrder {
        bool operator()(const Order l, const Order r) const {
            if (l.price != r.price) {
                return l.price > r.price;
            }
            return l.time > r.time; 
        }
    };

}


