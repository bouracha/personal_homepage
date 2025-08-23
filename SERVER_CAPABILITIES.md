# Server Capabilities Summary (salmon.maths.tcd.ie)

## Operating System / Environment

- **OS**: FreeBSD host, accessible via SSH
- **Shell**: tcsh
- **Uptime**: Stable (60+ days)

## Web Server

- **Apache HTTPD**: 2.4.62 (FreeBSD build)
- **Configuration**: Running with MPM prefork
- **Main DocumentRoot**: `/usr/local/www/apache24/data` (owned by root; not writable as normal user)
- **PHP Integration**: mod_php is loaded (php_module)
- **PHP-FPM**: Running on 127.0.0.1:9000 (but not in user PATH)

## Languages Installed

- **PHP**: 8.3.14 (CLI)
- **Python**: 3.11.10
- **Node.js**: ❌ Not available (`node: Command not found`)

## What Works ✅

- **User-space Python/Flask**: Can run web server on high port (e.g. 8082)
  - **Tested successfully**: Flask app returned "Hello from Flask!"
- **PHP**: Available for Apache execution, but requires write access to suitable docroot directory
  - Currently lacking permissions in `/usr/local/www/apache24/data`
- **SSH tunneling**: Works for accessing user-run apps if ports aren't externally reachable

## What Doesn't Work (yet) ⚠️

- **Apache docroot write access**: No permission to write directly into main Apache docroot
- **UserDir**: `~/public_html → http://…/~user/` is not enabled in Apache config
- **Node.js support**: No direct Node.js support available

## Paths Forward

### Option 1: Flask on High Port ✅
- Keep running Flask app on high port (`python app.py`)
- **Pros**: Works today without admin involvement
- **Access**: Can expose externally via SSH tunnel or ask admin to proxy from Apache

### Option 2: PHP via Apache ⚠️
- Run PHP in Apache, if admin grants write access to subdir in docroot
- **Available**: PHP already available via mod_php/php-fpm

### Option 3: UserDir Setup ⚠️
- Ask admin to enable UserDir so you can serve PHP from `~/public_html` over Apache on port 80

### Option 4: Apache Proxy ⚠️
- Ask admin to proxy `/myapp` in Apache to your Flask process (on 8082)
- Makes it accessible as part of the main site

## Conclusion

- ✅ **Dynamic content possible** with Python (Flask) or PHP
- ⚠️ **Direct Apache docroot write access** requires admin cooperation
- ❌ **Node.js not installed**
- ✅ **User-run servers on high ports** confirmed working

---

*Generated from domain investigation - reference for future development decisions*