const fs = require('fs')

const puppeteer = require('puppeteer');
const Redis = require('ioredis');
const connection = new Redis(6379, 'redis');

const browser_option = {
    executablePath: 'google-chrome-unstable',
    headless: true,
    args: [
        '--no-sandbox',
        '--disable-background-networking',
        '--disk-cache-dir=/dev/null',
        '--disable-default-apps',
        '--disable-extensions',
        '--disable-gpu',
        '--disable-sync',
        '--disable-translate',
        '--hide-scrollbars',
        '--metrics-recording-only',
        '--mute-audio',
        '--no-first-run',
        '--safebrowsing-disable-auto-update',
    ],
};

const crawl = async (url) => {
    console.log(`[*] started: ${url}`)
    
    let browser = await puppeteer.launch(browser_option);
    const page = await browser.newPage();
    await page.setCookie({
        name: 'flag',
        value: process.env.FLAG,
        domain: process.env.DOMAIN,
        expires: Date.now() / 1000 + 10,
    });
    try {
        await page.goto(url, {
            waitUntil: 'networkidle0',
            timeout: 5000,
        });
    } catch (err){
        console.log(err);
    }
    await page.close();
    await browser.close();
    console.log(`[*] finished: ${url}`)
};

// handle the whole
function handle(){
    console.log("[*] waiting new query ...")
    connection.blpop("query", 0, async function(err, message) {
        const url = message[1];
        await crawl(url);
        await connection.incr("proceeded_count");
        setTimeout(handle, 10);
    });
}
handle();
