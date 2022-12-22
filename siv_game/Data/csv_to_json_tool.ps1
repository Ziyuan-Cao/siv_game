<#
.SYNOPSIS
Converts .xlsx to .json
.PARAMETER input_file_name
The path of .xlsx
.PARAMETER column_start
which column is begining
.PARAMETER output_file_name
The path to a JSON file to be created.
.EXAMPLE
.\csv_to_json_tool .\MonsterData.xlsx 3
#>

[CmdletBinding()]
Param
(
    [Parameter(ValueFromPipeline=$true,Mandatory=$true)]
    [Object]$input_file_name,

    [Parameter(Mandatory=$true)]
    [int]$column_start = 1,

    [Parameter()]
    [string]$output_file_name
)

#auto downloal excel moduel 
if (Get-Module -ListAvailable -Name ImportExcel) 
{
    Write-Output "ImportExcel module exists"
} 
else 
{
    Write-Output "Module does not exist, auto download moduel"
    Install-Module -Name ImportExcel -Scope CurrentUser -Force
}

#input file path process
if($input_file_name -is "System.IO.FileSystemInfo")
{
    $input_file_name = $input_file_name.FullName.ToString()
}
if(-not(Test-Path -Path $input_file_name))
{
    throw "wrong input_file_name parameter"
}

#output file path process
$out_data_name = "empty name"
if (-not $output_file_name) 
{
    $output_file_name = [System.IO.Path]::GetFileNameWithoutExtension($(Split-Path $input_file_name -Leaf))
    $out_data_name = $output_file_name
    $output_file_name = Join-Path $pwd ($output_file_name + ".json")
}
$output_file_name = [System.IO.Path]::GetFullPath($output_file_name)

#inport .xlsx data only
$input_file_name = [System.IO.Path]::GetFullPath($input_file_name)
Write-Output "Converting '$input_file_name' to JSON"

$xlsx_data = Import-Excel -Path $input_file_name -StartRow $column_start -DataOnly

#convert xlsx to json encode utf8
$xlsx_data  = [pscustomobject]@{$out_data_name = $xlsx_data }

$json_data = ConvertTo-Json -InputObject $xlsx_data 
#$json_data = "{" + '"'+ "monster_list "+ '"' +": "  +$json_data + "}"

Set-Content $json_data -Encoding utf8 -Path $output_file_name



