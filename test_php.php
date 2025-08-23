<?php
echo "PHP is working! Current time: " . date('Y-m-d H:i:s');
echo "<br>PHP Version: " . phpversion();
echo "<br>Current directory: " . __DIR__;
echo "<br>Is data directory writable? " . (is_writable(__DIR__ . '/data') ? 'YES' : 'NO');
echo "<br>Is current directory writable? " . (is_writable(__DIR__) ? 'YES' : 'NO');

// Test file creation
$testFile = __DIR__ . '/test_write.txt';
if (file_put_contents($testFile, 'test') !== false) {
    echo "<br>File write test: SUCCESS";
    unlink($testFile); // Clean up
} else {
    echo "<br>File write test: FAILED";
}
?>