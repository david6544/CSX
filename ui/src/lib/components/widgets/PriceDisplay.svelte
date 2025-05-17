<script lang="ts">
  import { onMount, onDestroy } from 'svelte';
  import { tickerPrices, connectionStatus, initWebSocket } from '$lib/services/api';
  
  // Format price as dollars
  function formatPrice(price: number): string {
    return `$${(price / 100).toFixed(2)}`;
  }

  export const tickerNames : {[index : number]: string} = {
    0: 'BOOGLE',
    1: 'MACROSOFT',
    // Add more tickers as needed
  };
  
  // Connection status display
  $: status = $connectionStatus ? '✅ Connected' : '❌ Disconnected';
  
  // Connect on component mount
  onMount(() => {
    initWebSocket();
  });
</script>

<div class="price-display">
  <h2>Market Prices</h2>
  
  <div class="status-indicator {$connectionStatus ? 'connected' : 'disconnected'}">
    {status}
  </div>
  
  {#if Object.keys($tickerPrices).length === 0}
    <p>Waiting for price updates...</p>
  {:else}
    <table>
      <thead>
        <tr>
          <th>Ticker</th>
          <th>Price</th>
        </tr>
      </thead>
      <tbody>
        {#each Object.entries($tickerPrices) as [ticker, price]}
          <tr>
            <!-- Change "ticker" to the actual ticker name -->
            <td>{tickerNames[Number(ticker)] || `Unknown (${ticker})`}</td>
            <td class="price">{formatPrice(price)}</td>
          </tr>
        {/each}
      </tbody>
    </table>
  {/if}
</div>

<style>
  .price-display {
    padding: 15px;
    background-color: #f5f5f5;
    border-radius: 8px;
    margin-bottom: 20px;
  }
  
  h2 {
    margin-top: 0;
  }
  
  table {
    width: 100%;
    border-collapse: collapse;
  }
  
  th, td {
    text-align: left;
    padding: 8px;
    border-bottom: 1px solid #ddd;
  }
  
  .price {
    font-weight: bold;
    font-family: monospace;
  }
  
  .status-indicator {
    display: inline-block;
    padding: 4px 8px;
    border-radius: 4px;
    margin-bottom: 12px;
    font-weight: 500;
  }
  
  .connected {
    background-color: #d4edda;
    color: #155724;
  }
  
  .disconnected {
    background-color: #f8d7da;
    color: #721c24;
  }
</style>