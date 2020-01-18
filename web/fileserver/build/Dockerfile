FROM ruby:2.6.5-alpine

COPY src /app
RUN cd /app && \
    gem install bundler && \
    bundle install

CMD cd /app && echo "SECCON{You_are_the_Globbin'_Slayer}" > flag.txt && bundle exec ruby app.rb
