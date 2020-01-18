const Redis = require('ioredis');
const request = require('request-promise');
const connection = new Redis(6379, 'redis');

// init
connection.set('queued_count', 0);
connection.set('proceeded_count', 0);

// publish tasks to workers
(async () => {
    const app = require('express')();
    const port = 8080;

    var bodyParser = require('body-parser');
    app.use(bodyParser());
    
    app.post('/query', (req, res) => {
        try {
            if (req.body.url !== undefined && req.body.url != ''){
                connection.rpush('query', req.body.url).then(() => {
                    connection.incr('queued_count')
                }).then(() => {
                    console.log(`[*] Queried: ${req.body.url}`);                    
                });
            }
            res.send('Okay! I got it :-)');
        } catch (e){
            res.send('Umm, there is something wrong ...');
        }
    });
    
    app.listen(port, () => {
        console.log(`Publisher is listening on port ${port}!`);
    });
})();

// reporting
const pushgateway_url = "http://monitor.noc.seccon.jp:9091/metrics/job/spa_metric";

const report = async () => {
    const queued_count = await connection.get('queued_count');
    const proceeded_count = await connection.get('proceeded_count');
    const report_body = `
# TYPE queued_count gauge
# HELP queued_count
queued_count ${queued_count}


# TYPE proceeded_count gauge
# HELP proceeded_count
proceeded_count ${proceeded_count}
`;
    request.post(pushgateway_url, {
        body: report_body,
    }).then(() => {
        console.log(`[*] Succeeded in reporting the metrics: ${queued_count}, ${proceeded_count}`);        
    }).catch(() => {
        console.error(`[-] Failed to report the metrics: ${queued_count}, ${proceeded_count}`);
    })
    setTimeout(report, 1000 * 30);
};
setTimeout(report, 1000 * 30);
