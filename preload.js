const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
  startGame: () => ipcRenderer.send('start-game'),
  stopGame: () => ipcRenderer.send('stop-game'),
  onGameOutput: (callback) => ipcRenderer.on('game-output', (event, data) => callback(data)),
  onGameError: (callback) => ipcRenderer.on('game-error', (event, data) => callback(data)),
  onGameEnded: (callback) => ipcRenderer.on('game-ended', (event, code) => callback(code))
});
