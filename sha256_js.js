// SHA-256 hash in Node.js using built-in crypto module
const readline = require('readline');
const crypto = require('crypto');

const rl = readline.createInterface({
	input: process.stdin,
	output: process.stdout,
});

rl.question('Enter a string to hash with SHA-256: ', (input) => {
	const hash = crypto.createHash('sha256').update(input, 'utf8').digest('hex');
	console.log(hash);
	rl.close();
});
