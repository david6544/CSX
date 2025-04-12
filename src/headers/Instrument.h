#include<cinttypes>
#include<functional>

namespace instrument {
    
    typedef uint16_t ticker;
    
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
