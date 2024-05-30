'use strict'

const fs   = require('node:fs');
const path = require('path');

const cwd = process.cwd();


/**
 * Get a list of all files from dirname
 * 
 * @param {*} dirname 
 * @param {[fs.Dirent]} files 
 * @param {[fs.Dirent]} dirs 
 */
function readDir(dirname, files, dirs)
{
    fs.readdirSync(dirname, { withFileTypes: true }).forEach( item =>
    {
        // If it's part of .git/ or build/, ignore it
        if (item.name == ".git") return;
        else if (item.name == "build") return;
        else if (item.name == "configure_project.js") return;
        else if (item.name == "node_modules") return;
        else if (item.name == "package-lock.json") return;
        else if (item.name == "package.json") return;
        
        // Get the full name
        item.name = path.join(dirname, item.name);
        
        // If it's a directory, investigate (and log) it
        if (item.isDirectory())
        {
            dirs.push(item);
            readDir(item.name, files, dirs);
        }

        // If it's a file, add it to the list of files
        if (item.isFile()) files.push(item);
    });
}

function printHelp()
{
    console.log(
        `configure_project.js usage:
            node configure_project.js <project_name> <exec_name> [optional proj_number]
        
        project_name:
            The name of the project
            Replaces the RegEx rule: /<PROJECT_NAME>/g
        
        exec_name:
            What to call the default hello world executable
            Replaces the RegEx rule: /<EXEC>/g
        
        proj_number:
            The project number, used in ifndef statements
            Typically assigned a number between 100'000 - 999'999, can optionally be specified
            Replaces the RegEx rule: /__PROJID__/g
        
        `
    );
}

function main()
{
    if (process.argv.length != 3) { printHelp(); return 0; }
    
    /**
     * @type {[fs.Dirent]}
     */
    var files = [];
    /**
     * @type {[fs.Dirent]}
     */
    var dirs  = [];
    // Read the current working directory
    readDir(cwd, files, dirs);

    // Ask the user what they want as their project name to be
    /**
     * @type {string}
     */
    const projName = process.argv[1];
    const execName = process.argv[2];
    const projID   = Math.floor((Math.random() * 899999) + 100000);

    if (projName.includes(' ') || projName.includes('<') || projName.includes('>'))
    {
        console.error("Forbidden project name");
        return 1;
    }

    // Setup the patterns
    const patterns =
    [
        { pattern: /__PROJID__/g,     val: projID   },
        { pattern: /<PROJECT_NAME>/g, val: projName },
        { pattern: /<EXEC>/g,         val: execName }
    ];

    // Rename any dirs that match the patterns
    dirs.forEach(dir => 
    {
        patterns.forEach(pat => {
            // Check if this dir matches the pattern
            if (pat.pattern.test(dir.name))
            {
                var dirNew = dir;
                dirNew.name.replace(pat.pattern, pat.val);

                // fs.renameSync(dir, dirNew);
            }
        });
    });

    // Apply changes to directories in memory
    files.forEach(file =>
    {
        patterns.forEach(pat =>
        {
            if (pat.pattern.test(file.name)) file.name.replace(pat.pattern, pat.val);
        });
    });
    

    // Write this data to all of the files
    files.forEach(file =>
    {
        console.log(file);

        var f = fs.readFileSync(file.name, 'utf-8');

        patterns.forEach(pat => {
            f = f.replace(pat.pattern, pat.val);
        });
        
        // fs.writeFileSync(file.name, f);
    });

    console.log(`Configured projectID: ${projID} with name ${projName}!`);
    console.log(`The hello world executable is named "${execName}"`);

    return 0;
}

process.exit(main());
