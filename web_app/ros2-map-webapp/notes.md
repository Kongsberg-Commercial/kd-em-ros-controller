cd ros2-map-webapp

npm install

npm run dev

Known error: 
```
TypeError: crypto.hash is not a function

This is a Node.js compatibility issue.
```

1. check node.js version

if it is below v18.x.x, upgrade might be needed

2. upgrade node.js

- install nvm if not installed:
```
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.7/install.sh | bash
# Close and reopen your terminal, or run:
export NVM_DIR="$([ -z "${XDG_CONFIG_HOME-}" ] && printf %s "${HOME}/.nvm" || printf %s "${XDG_CONFIG_HOME}/nvm")"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"
```

- install Node.js 18 or newer:
```
nvm install 20
nvm use 20
```

evt. make it default 
``` 
nvm alias defaul 20
``` 

3. reinstall dependency
```
rm -rf node_modules package-lock.json
npm install
``` 