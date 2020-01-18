<?php
if($_SERVER['REMOTE_ADDR'] === gethostbyname('php-fpm')){
    echo "SECCON{what_a_easy_bypass_314208thg0n423g}";
} else {
    echo "Forbidden.";
    echo 'Your IP: '  . $_SERVER['REMOTE_ADDR'];
}
?>
