import Parser from './grammar/Parser';

const parser = new Parser();

parser.parseFile('test.pasm').then((result) => {
  console.log(result);
}).catch((err) => {
  console.error(err);
})
