##########
pkg install sudo
visudo

##########
sudo ntpdate tock.stdtime.gov.tw
sudo pkg install python3 py36-pip vim bash zsh curl git
sh -c "$(curl -fsSL https://raw.githubusercontent.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
vim ~/.zshrc
ZSH_THEME="agnoster"
export EDITOR=vim
. ~/.zshrc

##########
sudo gpart create -s gpt da1
sudo gpart create -s gpt da2
sudo gpart add -t freebsd-zfs -l ftp0 da1
sudo gpart add -t freebsd-zfs -l ftp1 da2

sudo zpool create mypool mirror /dev/gpt/ftp0 /dev/gpt/ftp1
sudo zfs set compression=gzip mypool
sudo zfs set mountpoint=none mypool
sudo zfs set canmount=off mypool
sudo zfs create -o mountpoint=/usr/home/ftp/upload  mypool/upload
sudo zfs create -o mountpoint=/usr/home/ftp/public  mypool/public
sudo zfs create -o mountpoint=/usr/home/ftp/hidden  mypool/hidden

##########
cd /usr/ports/ftp/pure-ftpd
sudo make install

sudo cp /usr/local/etc/pure-ftpd.conf.sample /usr/local/etc/pure-ftpd.conf
sudoedit /usr/local/etc/pure-ftpd.conf
PureDB                       /usr/local/etc/pureftpd.pdb
PamAuthentication            yes
AnonymousCanCreateDirs       yes
AntiWarez                    no
Umask                        113:002
NoChmod                      yes
CallUploadScript             yes
TLS                          2

##########
sudo pw useradd ftp -g ftp -s /sbin/nologin
sudo pw groupadd ftpvip
sudo pw useradd sysadm -g ftpvip -G ftpvip,ftp -d /home/ftp
sudo passwd sysadm
sudo pw useradd ftpvirtual -g ftpvip -G ftpvip,ftp -d /dev/null -s /sbin/nologin
sudo pure-pw useradd ftp-vip -u ftpvirtual -d /home/ftp -m

##########
cd /usr/home
sudo chown -R sysadm:ftpvip ftp
sudo chmod 775 ftp
cd /usr/home/ftp
sudo chown -R sysadm:ftpvip public upload hidden
sudo -u sysadm mkdir hidden/treasure
sudo chmod 2775 hidden/treasure
sudo chmod 2771 hidden
sudo chmod 2777 upload
sudo chmod 2775 public
sudo setfacl -m u:ftp:rwx:f:deny upload
sudo setfacl -m u:ftp:rwx:d:allow upload

##########
sudo mkdir -p /etc/ssl/private
sudo openssl dhparam -out /etc/ssl/private/pure-ftpd-dhparams.pem 2048
sudo openssl req -x509 -nodes -newkey rsa:2048 -sha256 -keyout \
       /etc/ssl/private/pure-ftpd.pem \
       -out /etc/ssl/private/pure-ftpd.pem
sudo chmod 600 /etc/ssl/private/*.pem

##########
sudoedit /usr/local/bin/zbackup
sudo chmod 755 /usr/local/bin/zbackup

##########
sudoedit /usr/local/etc/rc.d/ftp_watchd
sudo chmod 555 /usr/local/etc/rc.d/ftp_watchd

##########
sudo pip-3.6 install pyyaml schedule
sudoedit /usr/local/bin/zbackupd
sudo chmod 755 /usr/local/bin/zbackupd
sudoedit /usr/local/etc/rc.d/zbackupd
sudo chmod 555 /usr/local/etc/rc.d/zbackupd
sudoedit /usr/local/etc/zbackupd.yaml

##########
sudoedit /etc/rc.conf
pureftpd_enable="YES"
ftp_watchd_enable="YES"
ftp_watchd_command="echo 'uploaded' >> /tmp/ftp_watchd"
zbackupd_enable="YES"
