import win32com.client  # import win32com

projectName = "<path-to-project>\\RunAIMMS.aimms"
AIMMS = win32com.client.Dispatch("AimmsSelector.Selector32") 
project = AIMMS.GetAimmsProject(projectName)

project.StartupMode = 1  # STARTUP_MINIMIZED
project.ProjectOpen(projectName,0)

depots = project.GetSet("Depots")
customers = project.GetSet("Customers")

depots.ElementPassMode = 2 # ELEMENT_BY_NAME
depots.AssignElementArray(["d-1", "d-2", "d-3"], 0)

customers.ElementPassMode = 2 # ELEMENT_BY_NAME
customers.AssignElementArray(["c-1", "c-2", "c-3", "c-4", "c-5", "c-6", "c-7", "c-7", "c-8", "c-9", "c-10"], 0)

supply = project.GetIdentifier("Supply")
demand = project.GetIdentifier("Demand")
unitTransportCost = project.GetIdentifier("UnitTransportCost")

supply.AssignArray([ 10.0, 15.0, 20.0 ])
demand.AssignArray([3.0, 6.0, 3.0, 3.5, 5.5, 5.0, 6.0, 1.5, 7.0, 4.5])
unitTransportCost.AssignArray(
[ [ 1.0, 0.7, 0.3, 0.8, 1.2, 0.3, 1.1, 0.9, 1.0, 0.5 ],
  [ 0.7, 0.9, 1.3, 0.9, 0.9, 1.3, 1.2, 0.5, 0.7, 1.5 ],
  [ 1.2, 0.5, 0.9, 1.8, 1.1, 0.7, 0.5, 1.4, 0.5, 1.4 ] ])
  
project.GetProcedure("MainExecution").Run([])

transport = project.CreateArray("Transport", 0, 0)

status = project.GetScalar("TransportModelStatus").Value
cost = project.GetScalar("TotalCost").Value

