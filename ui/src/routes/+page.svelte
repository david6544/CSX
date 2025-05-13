<script>
    import { onMount } from 'svelte'
    import { initWebSocket, sendOrder } from '$lib/services/api';

    onMount(() => {
        initWebSocket();
    });

    // Mock data for demonstration
    const portfolio = {
        totalValue: 15420.65,
        dailyChange: 235.42,
        dailyChangePercent: 1.55,
        assets: [
            { symbol: 'AAPL', name: 'Apple Inc.', shares: 10, price: 178.42, value: 1784.20, change: 2.3 },
            { symbol: 'MSFT', name: 'Microsoft Corp.', shares: 5, price: 325.18, value: 1625.90, change: -0.8 },
            { symbol: 'GOOGL', name: 'Alphabet Inc.', shares: 3, price: 140.52, value: 421.56, change: 1.2 },
            { symbol: 'AMZN', name: 'Amazon.com Inc.', shares: 8, price: 172.36, value: 1378.88, change: 0.5 }
        ]
    };
    
    const trackedStocks = [
        { symbol: 'TSLA', name: 'Tesla, Inc.', price: 192.45, change: -1.2 },
        { symbol: 'NVDA', name: 'NVIDIA Corp.', price: 876.32, change: 3.5 },
        { symbol: 'META', name: 'Meta Platforms', price: 474.21, change: 2.1 },
        { symbol: 'DIS', name: 'Walt Disney Co.', price: 105.18, change: 0.3 }
    ];
    
    const tradeHistory = [
        { date: '2025-05-11', type: 'BUY', symbol: 'AAPL', shares: 5, price: 177.32, total: 886.60 },
        { date: '2025-05-09', type: 'SELL', symbol: 'MSFT', shares: 2, price: 329.85, total: 659.70 },
        { date: '2025-05-07', type: 'BUY', symbol: 'GOOGL', shares: 3, price: 138.24, total: 414.72 },
        { date: '2025-05-05', type: 'BUY', symbol: 'AMZN', shares: 4, price: 171.50, total: 686.00 }
    ];
    
    const marketOverview = {
        indices: [
            { name: 'S&P 500', value: 5328.42, change: 0.75 },
            { name: 'NASDAQ', value: 16782.65, change: 1.2 },
            { name: 'DOW', value: 38642.18, change: 0.3 }
        ],
        topGainers: [
            { symbol: 'NVDA', change: 3.5 },
            { symbol: 'META', change: 2.1 },
            { symbol: 'AAPL', change: 2.3 }
        ],
        topLosers: [
            { symbol: 'TSLA', change: -1.2 },
            { symbol: 'MSFT', change: -0.8 },
            { symbol: 'NFLX', change: -0.5 }
        ]
    };
    
    function handleOrderSubmit() {
        const newOrder = {
            price: 6969,
            quantity: 5,
        }
        sendOrder(newOrder);
        console.log('Order sent:', newOrder);
    }
</script>

<!-- Home Page-->


<section class="welcome-banner">
    <h1>Welcome to CSX Market Simulator</h1>
    <p>Track, trade and analyze your virtual portfolio</p>
</section>

<div class="dashboard">
    <!-- Portfolio Section -->
    <section class="card portfolio">
        <h2>Your Portfolio</h2>
        <div class="portfolio-summary">
            <div class="portfolio-total">
                <span class="label">Total Value</span>
                <span class="value">${portfolio.totalValue.toLocaleString()}</span>
            </div>
            <div class="portfolio-change" class:positive={portfolio.dailyChange > 0} class:negative={portfolio.dailyChange < 0}>
                <span class="value">${portfolio.dailyChange.toLocaleString()} ({portfolio.dailyChangePercent}%)</span>
                <span class="label">Today</span>
            </div>
        </div>

        <div class='action-button trade-action'>
            <button on:click={handleOrderSubmit}>Quick Trade</button>
        </div>
        
        <table class="holdings-table">
            <thead>
                <tr>
                    <th>Symbol</th>
                    <th>Company</th>
                    <th>Shares</th>
                    <th>Price</th>
                    <th>Value</th>
                    <th>Change</th>
                </tr>
            </thead>
            <tbody>
                {#each portfolio.assets as asset}
                    <tr>
                        <td class="symbol">{asset.symbol}</td>
                        <td>{asset.name}</td>
                        <td>{asset.shares}</td>
                        <td>${asset.price.toFixed(2)}</td>
                        <td>${asset.value.toFixed(2)}</td>
                        <td class:positive={asset.change > 0} class:negative={asset.change < 0}>
                            {asset.change > 0 ? '+' : ''}{asset.change}%
                        </td>
                    </tr>
                {/each}
            </tbody>
        </table>
    </section>

    <!-- Tracked Stocks Section -->
    <section class="card tracked-stocks">
        <h2>Tracked Stocks</h2>
        <table class="stocks-table">
            <thead>
                <tr>
                    <th>Symbol</th>
                    <th>Company</th>
                    <th>Price</th>
                    <th>Change</th>
                </tr>
            </thead>
            <tbody>
                {#each trackedStocks as stock}
                    <tr>
                        <td class="symbol">{stock.symbol}</td>
                        <td>{stock.name}</td>
                        <td>${stock.price.toFixed(2)}</td>
                        <td class:positive={stock.change > 0} class:negative={stock.change < 0}>
                            {stock.change > 0 ? '+' : ''}{stock.change}%
                        </td>
                    </tr>
                {/each}
            </tbody>
        </table>
        <div class="action-button">
            <button>Add Stock</button>
        </div>
    </section>

    <!-- Trade History Section -->
    <section class="card trade-history">
        <h2>Recent Trades</h2>
        <table class="trades-table">
            <thead>
                <tr>
                    <th>Date</th>
                    <th>Type</th>
                    <th>Symbol</th>
                    <th>Shares</th>
                    <th>Price</th>
                    <th>Total</th>
                </tr>
            </thead>
            <tbody>
                {#each tradeHistory as trade}
                    <tr>
                        <td>{trade.date}</td>
                        <td class:buy={trade.type === 'BUY'} class:sell={trade.type === 'SELL'}>
                            {trade.type}
                        </td>
                        <td class="symbol">{trade.symbol}</td>
                        <td>{trade.shares}</td>
                        <td>${trade.price.toFixed(2)}</td>
                        <td>${trade.total.toFixed(2)}</td>
                    </tr>
                {/each}
            </tbody>
        </table>
        <div class="action-button">
            <button>View All Trades</button>
        </div>
    </section>

    <!-- Market Overview Section -->
    <section class="card market-overview">
        <h2>Market Overview</h2>
        <div class="market-indices">
            {#each marketOverview.indices as index}
                <div class="index-card">
                    <h3>{index.name}</h3>
                    <p class="index-value">{index.value.toLocaleString()}</p>
                    <p class:positive={index.change > 0} class:negative={index.change < 0}>
                        {index.change > 0 ? '+' : ''}{index.change}%
                    </p>
                </div>
            {/each}
        </div>
        
        <div class="market-movers">
            <div class="gainers">
                <h3>Top Gainers</h3>
                <ul>
                    {#each marketOverview.topGainers as gainer}
                        <li>
                            <span class="symbol">{gainer.symbol}</span>
                            <span class="positive">+{gainer.change}%</span>
                        </li>
                    {/each}
                </ul>
            </div>
            <div class="losers">
                <h3>Top Losers</h3>
                <ul>
                    {#each marketOverview.topLosers as loser}
                        <li>
                            <span class="symbol">{loser.symbol}</span>
                            <span class="negative">{loser.change}%</span>
                        </li>
                    {/each}
                </ul>
            </div>
        </div>
    </section>
</div>

<style>
    
    .welcome-banner {
        text-align: center;
        margin-bottom: 30px;
        padding: 20px;
    }
    
    .welcome-banner h1 {
        font-size: 2.2rem;
        margin-bottom: 10px;
        color: #333;
    }
    
    .welcome-banner p {
        font-size: 1.2rem;
        color: #666;
    }
    
    .dashboard {
        display: grid;
        grid-template-columns: 1fr 1fr;
        gap: 20px;
    }
    
    .card {
        background: white;
        border-radius: 8px;
        box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        padding: 20px;
        margin-bottom: 20px;
    }
    
    .portfolio {
        grid-column: span 2;
    }
    
    .portfolio-summary {
        display: flex;
        justify-content: space-between;
        margin-bottom: 20px;
        background-color: #f8f9fa;
        padding: 15px;
        border-radius: 6px;
    }
    
    .portfolio-total, .portfolio-change {
        display: flex;
        flex-direction: column;
    }
    
    .portfolio-total .label, .portfolio-change .label {
        font-size: 0.9rem;
        color: #666;
    }
    
    .portfolio-total .value {
        font-size: 1.8rem;
        font-weight: bold;
        color: #333;
    }
    
    .portfolio-change .value {
        font-size: 1.4rem;
        font-weight: bold;
    }
    
    h2 {
        margin-top: 0;
        margin-bottom: 15px;
        font-size: 1.5rem;
        color: #333;
        border-bottom: 1px solid #eee;
        padding-bottom: 10px;
    }
    
    table {
        width: 100%;
        border-collapse: collapse;
    }
    
    th {
        text-align: left;
        padding: 10px;
        border-bottom: 2px solid #ddd;
        color: #555;
        font-weight: 600;
    }
    
    td {
        padding: 10px;
        border-bottom: 1px solid #eee;
    }
    
    .symbol {
        font-weight: bold;
        font-family: monospace;
    }
    
    .positive {
        color: #28a745;
    }
    
    .negative {
        color: #dc3545;
    }
    
    .buy {
        color: #28a745;
        font-weight: bold;
    }
    
    .sell {
        color: #dc3545;
        font-weight: bold;
    }
    
    .action-button {
        margin-top: 15px;
        text-align: center;
    }
    
    button {
        background-color: #007bff;
        color: white;
        border: none;
        padding: 8px 15px;
        border-radius: 4px;
        cursor: pointer;
        font-size: 0.9rem;
    }
    
    button:hover {
        background-color: #0069d9;
    }
    
    .market-indices {
        display: flex;
        justify-content: space-between;
        margin-bottom: 20px;
    }
    
    .index-card {
        background-color: #f8f9fa;
        padding: 15px;
        border-radius: 6px;
        text-align: center;
        width: 30%;
    }
    
    .index-card h3 {
        margin-top: 0;
        margin-bottom: 10px;
        font-size: 1rem;
    }
    
    .index-value {
        font-size: 1.2rem;
        font-weight: bold;
        margin: 5px 0;
    }
    
    .market-movers {
        display: flex;
        justify-content: space-between;
    }
    
    .gainers, .losers {
        width: 48%;
    }
    
    .gainers h3, .losers h3 {
        font-size: 1rem;
        margin-bottom: 10px;
    }
    
    ul {
        list-style-type: none;
        padding: 0;
    }
    
    li {
        display: flex;
        justify-content: space-between;
        padding: 5px 0;
        border-bottom: 1px solid #eee;
    }
    
    @media (max-width: 768px) {
        .dashboard {
            grid-template-columns: 1fr;
        }
        
        .portfolio {
            grid-column: span 1;
        }
        
        .market-indices {
            flex-direction: column;
        }
        
        .index-card {
            width: 100%;
            margin-bottom: 10px;
        }
        
        .market-movers {
            flex-direction: column;
        }
        
        .gainers, .losers {
            width: 100%;
            margin-bottom: 15px;
        }
    }
</style>

