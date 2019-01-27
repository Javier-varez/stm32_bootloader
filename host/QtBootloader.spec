# -*- mode: python -*-
import platform

block_cipher = None

a = Analysis(['QtBootloader.py'],
             pathex=['/Users/javier/Desktop/stm32_bootloader/host'],
             binaries=[],
             datas=[('res/mainwindow.ui', 'res')],
             hiddenimports=[],
             hookspath=[],
             runtime_hooks=[],
             excludes=[],
             win_no_prefer_redirects=False,
             win_private_assemblies=False,
             cipher=block_cipher,
             noarchive=False)
pyz = PYZ(a.pure, a.zipped_data,
             cipher=block_cipher)
exe = EXE(pyz,
          a.scripts,
          [],
          exclude_binaries=True,
          name='QtBootloader',
          debug=False,
          bootloader_ignore_signals=False,
          strip=False,
          upx=True,
          console=False )
coll = COLLECT(exe,
               a.binaries,
               a.zipfiles,
               a.datas,
               strip=False,
               upx=True,
               name='QtBootloader')

if platform.system() == 'Darwin':
    app = BUNDLE(coll,
                 name='QtBootloader.app',
                 icon=None,
                 bundle_identifier=None,
                 info_plist={
                     'NSHighResolutionCapable': 'True',
                     'NSRequiresAquaSystemAppearance': 'False'
                 })
