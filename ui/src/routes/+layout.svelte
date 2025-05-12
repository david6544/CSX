<script>
    import Navbar from "../lib/components/navbar/Navbar.svelte"
</script>

<!-- Global Layout -->
<Navbar title={"CSX"} />

<main>
    <!-- This is where page content will be injected -->
    <slot></slot>
</main>

<style>
    main {
        max-width: 1200px;
        margin: 0 auto;
        padding: 20px;
        font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Arial, sans-serif;
    }
</style>