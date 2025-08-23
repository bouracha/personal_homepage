<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST');
header('Access-Control-Allow-Headers: Content-Type');

// Check if this is a POST request
if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    http_response_code(405);
    echo json_encode(['success' => false, 'message' => 'Method not allowed']);
    exit;
}

// Spam protection - check honeypot field
if (!empty($_POST['website'])) {
    http_response_code(400);
    echo json_encode(['success' => false, 'message' => 'Spam detected']);
    exit;
}

// Get and sanitize form data
$fullName = trim($_POST['fullName'] ?? '');
$email = trim($_POST['email'] ?? '');
$interests = trim($_POST['interests'] ?? '');
$affiliation = trim($_POST['affiliation'] ?? '');

// Validate required fields
if (empty($fullName) || empty($email)) {
    http_response_code(400);
    echo json_encode(['success' => false, 'message' => 'Full name and email are required']);
    exit;
}

// Validate email format
if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
    http_response_code(400);
    echo json_encode(['success' => false, 'message' => 'Please enter a valid email address']);
    exit;
}

// Sanitize data for CSV
$fullName = str_replace(['"', ',', '\n', '\r'], ['""', '&#44;', ' ', ' '], $fullName);
$email = str_replace(['"', ',', '\n', '\r'], ['""', '&#44;', ' ', ' '], $email);
$interests = str_replace(['"', ',', '\n', '\r'], ['""', '&#44;', ' ', ' '], $interests);
$affiliation = str_replace(['"', ',', '\n', '\r'], ['""', '&#44;', ' ', ' '], $affiliation);

// Prepare CSV data
$timestamp = date('Y-m-d H:i:s');
$csvData = [
    $timestamp,
    $fullName,
    $email,
    $interests,
    $affiliation
];

// CSV file path
$csvFile = __DIR__ . '/data/mailing_list.csv';

// Ensure data directory exists
$dataDir = __DIR__ . '/data';
if (!is_dir($dataDir)) {
    if (!mkdir($dataDir, 0755, true)) {
        http_response_code(500);
        echo json_encode(['success' => false, 'message' => 'Unable to create data directory']);
        exit;
    }
}

// Check if CSV file exists, if not create it with headers
$fileExists = file_exists($csvFile);
if (!$fileExists) {
    $headers = ['Date', 'Full Name', 'Email', 'Interests', 'Affiliation'];
    $headerRow = '"' . implode('","', $headers) . '"' . "\n";
    if (file_put_contents($csvFile, $headerRow, LOCK_EX) === false) {
        http_response_code(500);
        echo json_encode(['success' => false, 'message' => 'Unable to create CSV file']);
        exit;
    }
}

// Check if email already exists (simple duplicate prevention)
if ($fileExists) {
    $existingData = file_get_contents($csvFile);
    if (strpos($existingData, $email) !== false) {
        echo json_encode(['success' => false, 'message' => 'This email address is already subscribed']);
        exit;
    }
}

// Append new subscription to CSV
$csvRow = '"' . implode('","', $csvData) . '"' . "\n";
if (file_put_contents($csvFile, $csvRow, FILE_APPEND | LOCK_EX) === false) {
    http_response_code(500);
    echo json_encode(['success' => false, 'message' => 'Unable to save subscription']);
    exit;
}

// Set appropriate file permissions (readable by owner, not by others)
chmod($csvFile, 0600);

// Success response
echo json_encode([
    'success' => true, 
    'message' => 'Thank you for subscribing! You\'ll receive updates on my latest research and AI art projects.'
]);
?>