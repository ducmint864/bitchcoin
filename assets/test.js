const request = require('request')
const cheerio = require('cheerio')
const fs = require('fs')

const url = "https://ycharts.com/indicators/bitcoin_average_transaction_fee"

function test() {
    request(url, (err, response, body) => {
        if (err) {
            console.log("Request failed fatally!\n")
            return;
        }
        
        // else
        console.log("Request succeeded!\n")

        const $ = cheerio.load(body)
        let transactionFee = $('td[class=text-right]').html().replaceAll(" ", "").replaceAll("\n", "")

        fs.writeFile("./fee", transactionFee, (err) => {
            if (err) {
                console.log("ERROR! CAN'T WRITE TO FILE! DO YOU HAVE THE RIGHT PERMISSION?\n")
                return;
            }

            console.log('WRITE SUCCEEDED!\n')
        })        
    })
}

// main
test()








