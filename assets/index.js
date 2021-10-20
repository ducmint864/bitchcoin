const request = require("request");
const fs = require("fs");
const cheerio = require('cheerio')
const { setFlagsFromString } = require("v8");
const events = require('events')
const eventEmitter = new events.EventEmitter()
// console.log = function() {} // hide all console output
// console.clear = function() {} //

// console.log('\nChoose fetching interval [milisecs] | *RECOMMENDED : 30000\n')
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
        console.log('\nFETCH CHING AGAIN IN ', milisecs / 1000, '[secs]\n');
        finishedCount = 0;
    }
}



//
function fetch()
{

// fetch bitcoin values
    request(valuesUrl, (err, response, body) => {
        if (err) {
            fs.truncate("./assets/fetcherState", 0, (err) => {});

            console.log("REQUEST TO '", valuesUrl, "' FATALLY FAILED!\n");
            fs.appendFile("./assets/fetcherState", "0", (err) => {
                if (err)
                {
                    console.log("Goddamn it!\n");
                    // return;
                }
            })

            eventEmitter.emit('finished');
            return;
        }

        // request success
        else if (!err) {
            console.log("REQUEST TO '", valuesUrl, "' SUCCESS!\n")

            // clear files everytime
            // fs.truncate("./assets/values", 0, (err) => {
            //     if (err) {
            //         console.log("ERROR! CAN'T CLEAR FILE!");
            //     }
            // })

        }
    })
    //

// fetch bitcon transaction fee
    request(feeUrl, (err, response, body) => {
        if (err) {
            fs.truncate("./assets/fetcherState", 0, (err)=> {});
            
            console.log("REQUEST TO '", feeUrl, "' FATALLY FAILED!\n")
            fs.appendFile("./assets/fetcherState", "0", (err) => {
                if (err)
                {
                    console.log("Goddamn it!\n");
                    // return;
                }
            })

            eventEmitter.emit('finished');
            return;
        }

        else if (!err) {
            console.log("REQUEST TO '", feeUrl, "' SUCCESS!\n")
            const $ = cheerio.load(body);
            let fee = $('td[class=text-right]').html().replace(/\s/g,'').replace(/(\r\n|\n|\r)/gm, "") // remove white spaces and line breaks
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
