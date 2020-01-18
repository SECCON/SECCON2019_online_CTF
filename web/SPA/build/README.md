# How to deploy

```
cp _env .env
docker-compose up -d --build --scale worker=8
```
