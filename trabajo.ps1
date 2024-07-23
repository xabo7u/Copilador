#Numero de Particiones
Get-Partition

#Tipo de sistema de archivo montado en C:
Get-Volume -DriveLetter C | Select-Object DriveType , FileSystemType

#Espacio utilizado:
Get-PSDrive C

#Memoria del sistema
Get-WmiObject Win32_PhysicalMemory | Select-Object Manufacturer, MemoryType, BankLabel, ConfiguredClockSpeed, DeviceLocator, @{Name="Capacity (MB)";Expression={[math]::truncate($_.Capacity / 1MB)}}, SerialNumber | Format-Table -AutoSize

#Direccion IP
Get-NetIPAddress | Where-Object { $_.AddressFamily -eq "IPv4" -and $_.InterfaceAlias -ne "Loopback Pseudo-Interface 1" } | Select-Object IPAddress

#Gateway 
Get-NetIPConfiguration | Select-Object -ExpandProperty IPv4DefaultGateway

#Servidor DNS
Get-DnsClientServerAddress | Select-Object -ExpandProperty ServerAddresses
