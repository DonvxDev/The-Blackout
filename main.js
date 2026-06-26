const { app, BrowserWindow, Menu, ipcMain } = require('electron');
const path = require('path');
const { spawn } = require('child_process');

let mainWindow;
let gameProcess;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1280,
    height: 720,
    minWidth: 800,
    minHeight: 600,
    icon: path.join(__dirname, 'assets/icon.png'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: false,
      contextIsolation: true
    }
  });

  mainWindow.loadFile('index.html');

  const menu = Menu.buildFromTemplate([
    {
      label: 'File',
      submenu: [
        {
          label: 'Exit',
          accelerator: 'CmdOrCtrl+Q',
          click: () => {
            app.quit();
          }
        }
      ]
    },
    {
      label: 'Help',
      submenu: [
        {
          label: 'About',
          click: () => {
            require('electron').dialog.showMessageBox(mainWindow, {
              type: 'info',
              title: 'About The Blackout',
              message: 'The Blackout v1.0.0',
              detail: 'A professional PC shooter game\nBuilt with C++ and Electron'
            });
          }
        }
      ]
    }
  ]);

  Menu.setApplicationMenu(menu);

  mainWindow.on('closed', () => {
    mainWindow = null;
    if (gameProcess) {
      gameProcess.kill();
    }
  });
}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});

ipcMain.on('start-game', (event) => {
  const gamePath = path.join(__dirname, 'game', 'game.exe');
  gameProcess = spawn(gamePath);

  gameProcess.stdout.on('data', (data) => {
    event.sender.send('game-output', data.toString());
  });

  gameProcess.stderr.on('data', (data) => {
    event.sender.send('game-error', data.toString());
  });

  gameProcess.on('close', (code) => {
    event.sender.send('game-ended', code);
  });
});

ipcMain.on('stop-game', (event) => {
  if (gameProcess) {
    gameProcess.kill();
    event.sender.send('game-stopped');
  }
});
