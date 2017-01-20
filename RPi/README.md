# Simulink Models for Raspberry Pi Boards
A collection of Simulink models that extend features of Hardware Support Package for Raspberry Pi.

## Requirements
* MATLAB and Simulink 2016a
* [Simulink Support Package for Raspberry Pi Hardware](https://www.mathworks.com/hardware-support/raspberry-pi-simulink.html)

## Preparation procedure
1. Download and install [Raspbian image](https://www.raspberrypi.org/downloads/).
2. Use **apt-get** command to update and upgrade to the latest software version.
3. Target Language Compiler of Raspberry Pi hardware does not support sub-directory. Therefore put every source/header files on the same directory as .slx/.m files. 

### Preparation for USB serial blocks
1. Plug device into USB port.
2. Verify whether device driver is properly detected and initiated with **dmesg** and **ls /dev/ttyACM**. 

### Preparation for WiFi
1. Use **apt-get** command to install **hostapd** and **dnsmasq** packages.
2. Edit /etc/dhcpcd.conf to add **denyinterfaces wlan0** at the bottom line.
3. Edit /etc/network/interfaces to configure wlan0 as static IP.

   ```bash
   allow-hotplug wlan0  
   iface wlan0 inet static  
      address 172.24.1.1
      netmask 255.255.255.0
      network 172.24.1.0
      broadcast 172.24.1.255
   #    wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf
   ```

4. Restart dhcpd service with **sudo service dhcpcd restart** command.
5. Restart wlan0 interface with **sudo ifdown wlan0; sudo ifup wlan0** command.
6. Add file /etc/hostapd/hostapd.conf to configure hostapd.

   ```bash
   # This is the name of the WiFi interface we configured above
   interface=wlan0

   # Use the nl80211 driver with the brcmfmac driver
   driver=nl80211

   # This is the name of the network
   ssid=Pi3-AP

   # Use the 2.4GHz band
   hw_mode=g

   # Use channel 6
   channel=6

   # Enable 802.11n
   ieee80211n=1

   # Enable WMM
   wmm_enabled=1

   # Enable 40MHz channels with 20ns guard interval
   ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]

   # Accept all MAC addresses
   macaddr_acl=0

   # Use WPA authentication
   auth_algs=1

   # Require clients to know the network name
   ignore_broadcast_ssid=0

   # Use WPA2
   wpa=2

   # Use a pre-shared key
   wpa_key_mgmt=WPA-PSK

   # The network passphrase
   wpa_passphrase=raspberry

   # Use AES, instead of TKIP
   rsn_pairwise=CCMP
   ``` 
   
7. Test configuration by **sudo /usr/sbin/hostapd /etc/hostapd/hostapd.conf** command. WiFi access point with **Pi3-AP** name should display.
8. Edit /etc/default/hostapd to start service automatically.

   ```bash 
   DAEMON_CONF="/etc/hostapd/hostapd.conf"
   ``` 

8. Backup old dnsmasq configuration.

   ```bash 
   sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
   ``` 

9. Edit new configuration **/etc/dnsmasq.conf** file.

   ```bash 
   interface=wlan0      # Use interface wlan0  
   listen-address=172.24.1.1 # Explicitly specify the address to listen on  
   bind-interfaces      # Bind to the interface to make sure we aren't sending things elsewhere  
   server=8.8.8.8       # Forward DNS requests to Google DNS  
   domain-needed        # Don't forward short names  
   bogus-priv           # Never forward addresses in the non-routed address spaces.  
   dhcp-range=172.24.1.50,172.24.1.150,12h # Assign IP addresses between 172.24.1.50 and 172.24.1.150 with a 12 hour lease time  
   ```    
  
10.  Edit /etc/sysctl.conf to enable IPv4 forwarding by removing comment from the following line.

   ```bash
   # Uncomment the next line to enable packet forwarding for IPv4
   net.ipv4.ip_forward=1
   ```    

11. Configure NAT service to bridge Internet connection.

   ```bash
   sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE  
   sudo iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT  
   sudo iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT  
   ```
   
12. Store IP table for later use by **sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"** command.
13. Append command into /etc/rc.local to run every reboot.

   ```bash
   iptables-restore < /etc/iptables.ipv4.nat  
   service dnsmasq start 
   ```
   
14. Reboot.

## Blocks
1. **ttyACM Serial Receive** and **ttyACM Serial Transmit** blocks for serial communication with USB devices.
2. **ttyACM Serial Frame Receive** and **ttyACM Serial Frame Transmit** blocks for serial communication in frame format (start of frame/end of frame) with USB devices.

## References
1. [APT Raspberry Pi Documentation](https://www.raspberrypi.org/documentation/linux/software/apt.md).
2. [Serial Programming/termios](https://en.wikibooks.org/wiki/Serial_Programming/termios).
3. [USING YOUR NEW RASPBERRY PI 3 AS A WIFI ACCESS POINT WITH HOSTAPD](https://frillip.com/using-your-raspberry-pi-3-as-a-wifi-access-point-with-hostapd/).
