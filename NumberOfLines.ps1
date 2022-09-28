dir -Recurse *.h | Get-Content | Measure-Object -Line

Read-Host -Prompt "Press Enter to exit"