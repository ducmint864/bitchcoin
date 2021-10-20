const request = require("request");
const fs = require("fs");
const cheerio = require('cheerio')
const { setFlagsFromString } = require("v8");
const events = require('events')
const eventEmitter = new events.EventEmitter()

const valuesUrl = 'https://blockchain.info/de/ticker'
const feeUrl = 'https://ycharts.com/indicators/bitcoin_average_transaction_fee'

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
            fs.truncate("./assets/values", 0, (err) => {
                if (err) {
                    console.log("ERROR! CAN'T CLEAR FILE!");
                }
            })

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

fetch()