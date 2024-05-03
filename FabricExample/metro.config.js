const {getDefaultConfig, mergeConfig} = require('@react-native/metro-config');

/**
 * Metro configuration
 * https://facebook.github.io/metro/docs/configuration
 *
 * @type {import('metro-config').MetroConfig}
 */

/* eslint-disable import/no-commonjs */

const fs = require('fs');
const path = require('path');
const exclusionList = require('metro-config/src/defaults/exclusionList');
const escape = require('escape-string-regexp');
const pack = require('../package.json');

const root = path.resolve(__dirname, '..');

const modules = [
  ...Object.keys(pack.peerDependencies),
];

const rnwPath = fs.realpathSync(
  path.resolve(require.resolve('react-native-windows/package.json'), '..'),
);
const config = {
  projectRoot: __dirname,
  watchFolders: [root],

  // We need to make sure that only one version is loaded for peerDependencies
  // So we block them at the root, and alias them to the versions in example's node_modules
  resolver: {
    blacklistRE: exclusionList(
      modules.map(
        (m) =>
          new RegExp(`^${escape(path.join(root, 'node_modules', m))}\\/.*$`)
      ).concat([
        // This stops "react-native run-windows" from causing the metro server to crash if its already running
        new RegExp(
          `${path.resolve(__dirname, 'windows').replace(/[/\\]/g, '/')}.*`,
        ),
        // This prevents "react-native run-windows" from hitting: EBUSY: resource busy or locked, open msbuild.ProjectImports.zip or other files produced by msbuild
        new RegExp(`${rnwPath}/build/.*`),
        new RegExp(`${rnwPath}/target/.*`),
        /.*\.ProjectImports\.zip/,
      ])
    ),

    extraNodeModules: modules.reduce((acc, name) => {
      acc[name] = path.join(__dirname, 'node_modules', name);
      return acc;
    }, {}),
  },

  transformer: {
    getTransformOptions: async () => ({
      transform: {
        experimentalImportSupport: false,
        inlineRequires: true,
      },
    }),
  },
};

module.exports = mergeConfig(getDefaultConfig(__dirname), config);
