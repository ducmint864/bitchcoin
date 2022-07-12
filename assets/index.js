const request = require("request");
const fs = require("fs");
const cheerio = require('cheerio')
const { setFlagsFromString } = require("v8");
const prompt = require('prompt-sync')({sigint : true});
const events = require('events')
const eventEmitter = new events.EventEmitter()
// console.log = function() {} // hide all console output
// console.clear = function() {} //

// console.log('\nChoose fetching interval [milisecs] | *RECOMMENDED : 30000\n')
// const milisecs = prompt(">>> ");
const milisecs = 30000;


// global vars
const valuesUrl = 'https://blockchain.info/de/ticker'
const feeUrl = 'https://ycharts.com/indicators/bitcoin_average_transaction_fee'
var finishedCount = 0;



//
function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}



function finishedHandler() {
    ++finishedCount;
    if (finishedCount == 2) {
        console.log('\nFetching again in', milisecs / 1000, '[secs]\n');
        finishedCount = 0;
    }
}



//
function fetch()
{

// fetch bitcoin values
    request(valuesUrl, (err, response, body) => {
        if (err) {
            console.log("REQUEST TO '", valuesUrl, "' FATALLY FAILED!\n");

            eventEmitter.emit('finished');
            return;
        }

        // request success
        else if (!err) {
            console.log("REQUEST TO '", valuesUrl, "' SUCCESS!\n")

            // clear files everytime
            fs.truncate("./assets/values", 0, (err) => {
                if (err) {
                    console.log("ERROR! CAN'T CLEAR FILE!");
                }
            })

            // parse the raw strings into json type in javascript
            let jsonObj = JSON.parse(body);

            // loop through keys of json objects
            Object.keys(jsonObj).forEach( (key) => {
                let buyValue = parseInt(jsonObj[key].buy, 10);
                let sellValue = parseInt(jsonObj[key].sell, 10);

                fs.appendFile("./assets/values", (key.toString() + ' ' + buyValue.toString() + ' ' + sellValue.toString() + '\n'), (err) => {
                    if (err) {
                        console.log("ERROR! CAN'T WRITE VALUES TO FILE! DO YOU HAVE THE RIGHT PERMISSION?"); 
                    }
                })
            })

            console.log('*WRITE VALUES TO FILE SUCCESS!\n')
            eventEmitter.emit('finished')
        }
    })
    //

// fetch bitcon transaction fee
    request(feeUrl, (err, response, body) => {
        if (err) {
            console.log("REQUEST TO '", feeUrl, "' FATALLY FAILED!\n")
            eventEmitter.emit('finished');
            return;
        }

        else if (!err) {
            console.log("REQUEST TO '", feeUrl, "' SUCCESS!\n")
            const $ = cheerio.load(body);
            let fee = $('td[class=text-right]').html().trim()
            fs.writeFile('./assets/fee', fee, (err) => {
                if (err) {
                    console.log("ERROR! CAN'T WRITE FEE TO FILE! DO YOU HAVE THE RIGHT PERMISSION?\n") 
                    return;
                }

                // else
            })
            
            console.log('*WRITE FEE TO FILE SUCCESS!\n')
            eventEmitter.emit('finished')
        }

    })
    //

}



// main
console.clear()
eventEmitter.addListener('finished', finishedHandler)
var count = 0;

async function main(ms) {
    while (true) {
        ++count;
        if (count % 3 == 0) {
            console.clear();
        }
        console.log("\t------------------------------------- Fetch number ", count, " -------------------------------------")
        fetch();   
        await sleep(ms);
    }
}

console.log("*Program started\n");
main(milisecs);
