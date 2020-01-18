FROM php:7.2-fpm-alpine

# Install extensions
RUN docker-php-ext-install pdo pdo_mysql

# Use the default production configuration
RUN mv "$PHP_INI_DIR/php.ini-production" "$PHP_INI_DIR/php.ini"

