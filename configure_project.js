'use strict'

const fs = require('fs');
const path = require('path');

const cwd = process.cwd();
const { stdin, stdout } = process;

/**
 * Taken directly from https://stackoverflow.com/questions/17837147/user-input-in-node-js
 * @param {string} question
 */
function prompt(question)
{
	return new Promise((resolve, reject) =>
	{
		stdin.resume();
		stdout.write(question);

		stdin.on('data', data => resolve(data.toString().trim()));
		stdin.on('error', err => reject(err));
  	});
}


/**
 * Get a list of all files from dirname
 * 
 * @param {*} dirname 
 * @param {[fs.Dirent]} files 
 */
function readDir(dirname, files)
{
    fs.readdirSync(dirname, { withFileTypes: true }).forEach( item =>
    {
        // If it's part of .git/ or build/, ignore it
        if (item.name == ".git") return;
        if (item.name == "build") return;
        if (item.name == "configure_project.js") return;
        
        // Get the full name
        item.name = path.join(dirname, item.name);
        
        // If it's a directory, investigate it
        if (item.isDirectory()) readDir(item.name, files);

        // If it's a file, add it to the list of files
        if (item.isFile()) files.push(item);
    });
}

async function main()
{
    // Read the current working directory
	/**
	 * @type {[fs.Dirent]}
	 */
    var files = [];
    readDir(cwd, files);

    // Ask the user what they want as their project name to be
	/**
	 * @type {string}
	 */
	const projName = await prompt("What's the project name? ");
	const projID = Math.floor((Math.random() * 899999) + 100000);

	if (projName.includes(' ') || projName.includes('<') || projName.includes('>'))
	{
		console.error("Forbidden project name");
		process.exit(1);
	}

	// Write this data to all of the files
	files.forEach(file =>
	{
		var f = fs.readFileSync(file.name, 'utf-8');

		f = f.replace(/__PROJID__/g, projID);
		f = f.replace(/<PROJECT_NAME>/g, projName);

		fs.writeFileSync(file.name, f);
	});

	console.log(`Configured projectID: ${projID} with name ${projName}!`);

	process.exit();
}

main();
