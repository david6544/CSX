<script>
    export let title;
    
    // Example navigation items - replace with your actual routes
    const navItems = [
        { name: 'Home', href: '/' },
        { name: 'Dashboard', href: '/dashboard' },
        { name: 'Stocks', href: '/stocks'}
    ];
</script>

<div class="navbar">
    <div class="navbar-brand">
        <h1>{title}</h1>
    </div>
    
    <div class="navbar-links">
        {#each navItems as item}
            <a href={item.href}>{item.name}</a>
        {/each}
    </div>
    
    <div class="navbar-user">
        <button class="user-button">Profile</button>
        <div class="user-dropdown">
            <!-- User dropdown content -->
            <a href="/profile">My Profile</a>
            <a href="/settings">Settings</a>
            <a href="/logout">Logout</a>
        </div>
    </div>
</div>

<style>
    .navbar {
        display: flex;
        align-items: center;
        justify-content: space-between;
        padding: 0 1rem;
        background-color: #282003;
        color: white;
        height: 60px;
        box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
        font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Arial, sans-serif;
    }
    
    .navbar-brand h1 {
        margin: 0;
        font-size: 1.5rem;
    }
    
    .navbar-links {
        display: flex;
        gap: 1.5rem;
    }
    
    .navbar-links a {
        color: #ecf0f1;
        text-decoration: none;
        font-weight: 500;
        transition: color 0.3s ease;
    }
    
    .navbar-links a:hover {
        color: #3498db;
    }
    
    .navbar-user {
        position: relative;
    }
    
    .user-button {
        background-color: #3498db;
        color: white;
        border: none;
        padding: 0.5rem 1rem;
        border-radius: 4px;
        cursor: pointer;
    }
    
    .user-dropdown {
        position: absolute;
        right: 0;
        top: 100%;
        background-color: white;
        border-radius: 4px;
        box-shadow: 0 2px 10px rgba(0, 0, 0, 0.2);
        min-width: 150px;
        display: none;
        z-index: 100;
    }
    
    .navbar-user:hover .user-dropdown {
        display: flex;
        flex-direction: column;
    }
    
    .user-dropdown a {
        color: #333;
        padding: 0.75rem 1rem;
        text-decoration: none;
        transition: background-color 0.2s;
    }
    
    .user-dropdown a:hover {
        background-color: #f5f5f5;
    }
</style>