#!/usr/bin/env python3
import http.server
import socketserver
import urllib.parse
import json
import csv
import os
from datetime import datetime
from pathlib import Path

class MailingListHandler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/subscribe.php':
            self.handle_subscription()
        else:
            self.send_error(404)
    
    def handle_subscription(self):
        try:
            # Parse POST data
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length).decode('utf-8')
            
            # Debug: Print received data
            print(f"Received POST data: {post_data}")
            print(f"Content-Type: {self.headers.get('Content-Type', 'Not set')}")
            
            # Handle FormData vs URL-encoded data
            if 'multipart/form-data' in self.headers.get('Content-Type', ''):
                # This is FormData from JavaScript
                # We need to parse multipart data - let's use a simple approach
                form_data = {}
                # For now, let's convert JavaScript to use URLSearchParams instead
                self.send_json_response({'success': False, 'message': 'Please refresh the page and try again'}, 400)
                return
            else:
                # URL-encoded form data
                form_data = urllib.parse.parse_qs(post_data)
            
            print(f"Parsed form data: {form_data}")
            
            # Extract form fields
            full_name = form_data.get('fullName', [''])[0].strip() if form_data.get('fullName') else ''
            email = form_data.get('email', [''])[0].strip() if form_data.get('email') else ''
            interests = form_data.get('interests', [''])[0].strip() if form_data.get('interests') else ''
            affiliation = form_data.get('affiliation', [''])[0].strip() if form_data.get('affiliation') else ''
            website = form_data.get('website', [''])[0].strip() if form_data.get('website') else ''
            
            print(f"Extracted: name='{full_name}', email='{email}'")
            
            # Spam protection (honeypot)
            if website:
                self.send_json_response({'success': False, 'message': 'Spam detected'}, 400)
                return
            
            # Validate required fields
            if not full_name or not email:
                self.send_json_response({'success': False, 'message': 'Full name and email are required'}, 400)
                return
            
            # Basic email validation
            if '@' not in email or '.' not in email.split('@')[-1]:
                self.send_json_response({'success': False, 'message': 'Please enter a valid email address'}, 400)
                return
            
            # Create data directory if it doesn't exist
            data_dir = Path('./data')
            data_dir.mkdir(exist_ok=True)
            
            csv_file = data_dir / 'mailing_list.csv'
            
            # Check if file exists and read existing data
            file_exists = csv_file.exists()
            if file_exists:
                with open(csv_file, 'r', newline='') as f:
                    reader = csv.reader(f)
                    for row in reader:
                        if len(row) >= 3 and row[2] == email:  # Email is in column 2
                            self.send_json_response({'success': False, 'message': 'This email address is already subscribed'})
                            return
            
            # Prepare CSV data
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            csv_data = [timestamp, full_name, email, interests, affiliation]
            
            # Write to CSV
            with open(csv_file, 'a', newline='') as f:
                writer = csv.writer(f)
                if not file_exists:
                    # Write headers if new file
                    writer.writerow(['Date', 'Full Name', 'Email', 'Interests', 'Affiliation'])
                writer.writerow(csv_data)
            
            print(f"New subscription: {full_name} <{email}>")
            self.send_json_response({
                'success': True, 
                'message': 'Thank you for subscribing! You\'ll receive updates on my latest research and AI art projects.'
            })
            
        except Exception as e:
            print(f"Error handling subscription: {e}")
            self.send_json_response({'success': False, 'message': 'An error occurred. Please try again later.'}, 500)
    
    def send_json_response(self, data, status=200):
        self.send_response(status)
        self.send_header('Content-type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'POST')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
        self.wfile.write(json.dumps(data).encode())

if __name__ == "__main__":
    PORT = 8000
    Handler = MailingListHandler
    
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        print(f"Server running at http://localhost:{PORT}")
        print("This server can handle both static files AND the mailing list form!")
        print("Press Ctrl+C to stop")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped")