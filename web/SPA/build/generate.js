const axios = require('axios');
const {promises: fs} = require('fs');

const api = (path = '') => (
	axios({
		method: 'get',
		url: `https://api.github.com/repos/SECCON/SECCON2018_online_CTF/contents/${path}`,
		headers: {
			'User-Agent': 'hakatashi',
			Authorization: `token ${process.env.GITHUB_TOKEN}`,
		},
	})
);

(async () => {
	/*
	const {data: dirs} = await api('');
	const flags = [];
	for (const dir of dirs) {
		if (dir.type === 'dir') {
			const genre = dir.name;
			const {data: chals} = await api(genre);
			for (const chal of chals) {
				if (chal.type === 'dir') {
					const point = null;
					const name = chal.name.replace(/_/g, ' ');
					const data = await api(`${chal.path}/flag.txt`).catch(() => null);
					let flag = null;
					if (data !== null) {
						flag = Buffer.from(data.data.content, 'base64').toString().trim();
					} else {
						const data2 = await api(`${chal.path}/answer.txt`).catch(() => null);
						if (data2 !== null) {
							flag = Buffer.from(data2.data.content, 'base64').toString().trim();
						}
					}
					console.log(flag);
					flags.push({
						genre,
						name,
						point,
						flag,
					});
				}
			}
		}
	}
	*/
	await fs.writeFile('html/SECCON2019.json', JSON.stringify({
		flags: [{
			genre: 'Web',
			name: 'SPA',
			point: null,
			flag: '?????',
		}],
		name: 'SECCON CTF 2019 Quals',
		ctftime: 'https://ctftime.org/event/799',
		date: {
			start: new Date('2018-10-27T15:00:00+0900').getTime(),
			end: new Date('2018-10-27:00:00+0900').getTime(),
		},
	}, null, '  '));
})();