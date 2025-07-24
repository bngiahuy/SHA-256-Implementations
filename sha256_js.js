// SHA-256 hash in Node.js using built-in crypto module
const crypto = require('crypto');

function hashWithSHA256(input) {
	return crypto.createHash('sha256').update(input, 'utf8').digest('hex');
}

const input = "your string here";
console.log(hashWithSHA256(input));
