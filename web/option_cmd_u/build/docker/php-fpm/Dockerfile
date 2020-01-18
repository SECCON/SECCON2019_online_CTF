FROM php:7.2.0-fpm-alpine

RUN apk add --no-cache icu-dev
RUN docker-php-ext-install intl

ADD www.conf /usr/local/etc/php-fpm.d/www.conf