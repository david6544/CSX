#include<cinttypes>

namespace instrument {
    
    struct Instrument {
        uint16_t name;
    };

    struct Order {
        uint64_t time;   
        uint16_t price;
        uint16_t quantity;
    };
        
    struct OrderMatch {
        bool operator()(const Order l, const Order r) const {
            if (l.price != r.price) {
                return l.price < r.price;
            }
            return l.time > r.time; 
        }
    };

}


