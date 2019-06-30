
exports.log = log;
exports.error = error;
exports.downloadAndUnzip = downloadAndUnzip;
exports.cleanup = cleanup;

function exec(command, errorMessage) {
    
} 

function log(message) {
    console.log(createLogMessage(message))
}

function error(err) {
    console.error(createLogMessage(err));
    exitWithError();
}

function downloadAndUnzip() {

}

function cleanup() {

}

function exitWithError() {
    cleanup();
    process.exit(1);
}

function createLogMessage(message) {
    return typeof message === "string" ? `### ${message}` : message;
}