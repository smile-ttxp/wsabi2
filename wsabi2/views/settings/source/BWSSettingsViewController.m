// This software was developed at the National Institute of Standards and
// Technology (NIST) by employees of the Federal Government in the course
// of their official duties. Pursuant to title 17 Section 105 of the
// United States Code, this software is not subject to copyright protection
// and is in the public domain. NIST assumes no responsibility whatsoever for
// its use by other parties, and makes no guarantees, expressed or implied,
// about its quality, reliability, or any other characteristic.

#import "BWSSettingsViewController.h"

#import "BWSConstants.h"
#import "BWSSettingsPendingOperationsViewController.h"
#import "BWSSettingsShowSensorsViewController.h"
#import "UITableView+BWSUtilities.h"

/// Tag for the switch in the table for touch logging
static const NSUInteger kWSSettingsLoggingTouchLoggingSwitchTag = 1;
/// Tag for the switch in the table for motion logging
static const NSUInteger kWSSettingsLoggingMotionLoggingSwitchTag = 2;
/// Tag for the switch in the table for network logging
static const NSUInteger kWSSettingsLoggingNetworkLoggingSwitchTag = 3;
/// Tag for the switch in the table for device debug logging
static const NSUInteger kWSSettingsLoggingDeviceLoggingSwitchTag = 4;
/// Tag for the switch in the table for showing the logging panel
static const NSUInteger kWSSettingsLoggingShowLoggingPanelSwitchTag = 5;
/// Tag for the switch in the table for verbose logging
static const NSUInteger kWSSettingsLoggingVerboseLoggingSwitchTag = 6;

@interface BWSSettingsViewController ()

/// Persist settings based on switch toggle
- (IBAction)switchToggledForSwitch:(UISwitch *)sender;

@end

@implementation BWSSettingsViewController

#pragma mark - View Lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.contentSizeForViewInPopover = CGSizeMake(self.tableView.frame.size.width, [self.tableView contentHeight]);
    self.navigationController.contentSizeForViewInPopover = CGSizeMake(self.tableView.frame.size.width, [self.tableView contentHeight]);
    
    [[self navigationItem] setTitle:NSLocalizedString(kWSSettingsLabel, nil)];
}

- (void)viewWillAppear:(BOOL)animated
{
    self.contentSizeForViewInPopover = CGSizeMake(self.tableView.frame.size.width, [self.tableView contentHeight]);
    self.navigationController.contentSizeForViewInPopover = CGSizeMake(self.tableView.frame.size.width, [self.tableView contentHeight]);
    
    [super viewWillAppear:animated];
}

#pragma mark - TableView Delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.section) {
        case kWSSettingsSectionLogging:
            switch (indexPath.row) {
                case kWSSettingsLoggingShowSavedLogsRow:
                    break;
                default:
                    [tableView deselectRowAtIndexPath:indexPath animated:NO];
                    break;
            }
            break;
        case kWSSettingsSectionSensors:
            switch (indexPath.row) {
                case kWSSettingsSensorsShowSensorsRow: {
                    BWSSettingsShowSensorsViewController *sensorsVC = [[BWSSettingsShowSensorsViewController alloc] initWithNibName:@"BWSSettingsShowSensorsView" bundle:nil];
                    [[self navigationController] pushViewController:sensorsVC animated:YES];
                    break;
                }
                case kWSSettingsSensorsShowPendingOperationsRow: {
                    BWSSettingsPendingOperationsViewController *pendingVC = [[BWSSettingsPendingOperationsViewController alloc] initWithStyle:UITableViewStylePlain];
                    [[self navigationController] pushViewController:pendingVC animated:YES];
                    break;
                }
                default:
                    [tableView deselectRowAtIndexPath:indexPath animated:NO];
                    break;
            }
            break;
    }
}

#pragma mark - TableView Data Source

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString * const cellIdentifier = @"SettingsCell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (cell == nil)
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellIdentifier];
    
    UISwitch *settingSwitch = nil;
    switch (indexPath.section) {
        case kWSSettingsSectionLogging:
            switch (indexPath.row) {
                case kWSSettingsLoggingTouchLoggingRow:
                    [[cell textLabel] setText:kWSSettingsLoggingTouchLoggingRowLabel];
                    settingSwitch = [[UISwitch alloc] init];
                    [settingSwitch setOn:[[NSUserDefaults standardUserDefaults] boolForKey:kSettingsTouchLoggingEnabled]];
                    [settingSwitch addTarget:self action:@selector(switchToggledForSwitch:) forControlEvents:UIControlEventValueChanged];
                    [settingSwitch setTag:kWSSettingsLoggingTouchLoggingSwitchTag];
                    [cell setAccessoryView:settingSwitch];
                    break;
                case kWSSettingsLoggingMotionLoggingRow:
                    [[cell textLabel] setText:kWSSettingsLoggingMotionLoggingRowLabel];
                    settingSwitch = [[UISwitch alloc] init];
                    [settingSwitch setOn:[[NSUserDefaults standardUserDefaults] boolForKey:kSettingsMotionLoggingEnabled]];
                    [settingSwitch addTarget:self action:@selector(switchToggledForSwitch:) forControlEvents:UIControlEventValueChanged];
                    [settingSwitch setTag:kWSSettingsLoggingMotionLoggingSwitchTag];
                    [cell setAccessoryView:settingSwitch];
                    break;
                case kWSSettingsLoggingNetworkLoggingRow:
                    [[cell textLabel] setText:kWSSettingsLoggingNetworkLoggingRowLabel];
                    settingSwitch = [[UISwitch alloc] init];
                    [settingSwitch setOn:[[NSUserDefaults standardUserDefaults] boolForKey:kSettingsNetworkLoggingEnabled]];
                    [settingSwitch addTarget:self action:@selector(switchToggledForSwitch:) forControlEvents:UIControlEventValueChanged];
                    [settingSwitch setTag:kWSSettingsLoggingNetworkLoggingSwitchTag];
                    [cell setAccessoryView:settingSwitch];
                    break;
                case kWSSettingsLoggingDeviceLoggingRow:
                    [[cell textLabel] setText:kWSSettingsLoggingDeviceLoggingRowLabel];
                    settingSwitch = [[UISwitch alloc] init];
                    [settingSwitch setOn:[[NSUserDefaults standardUserDefaults] boolForKey:kSettingsDeviceLoggingEnabled]];
                    [settingSwitch addTarget:self action:@selector(switchToggledForSwitch:) forControlEvents:UIControlEventValueChanged];
                    [settingSwitch setTag:kWSSettingsLoggingDeviceLoggingSwitchTag];
                    [cell setAccessoryView:settingSwitch];
                    break;
                case kWSSettingsLoggingVerboseLoggingRow:
                    [[cell textLabel] setText:kWSSettingsLoggingVerboseLoggingRowLabel];
                    settingSwitch = [[UISwitch alloc] init];
                    [settingSwitch setOn:[[NSUserDefaults standardUserDefaults] boolForKey:kSettingsVerboseLoggingEnabled]];
                    [settingSwitch addTarget:self action:@selector(switchToggledForSwitch:) forControlEvents:UIControlEventValueChanged];
                    [settingSwitch setTag:kWSSettingsLoggingVerboseLoggingSwitchTag];
                    [cell setAccessoryView:settingSwitch];
                    break;
                case kWSSettingsLoggingShowLoggingPanelRow:
                    [[cell textLabel] setText:kWSSettingsLoggingShowLoggingPanelRowLabel];
                    settingSwitch = [[UISwitch alloc] init];
                    [settingSwitch setOn:[[NSUserDefaults standardUserDefaults] boolForKey:kSettingsLoggingPanelEnabled]];
                    [settingSwitch addTarget:self action:@selector(switchToggledForSwitch:) forControlEvents:UIControlEventValueChanged];
                    [settingSwitch setTag:kWSSettingsLoggingShowLoggingPanelSwitchTag];
                    [cell setAccessoryView:settingSwitch];
                    break;
                case kWSSettingsLoggingShowSavedLogsRow:
                    [[cell textLabel] setText:kWSSettingsLoggingShowSavedLogsRowLabel];
                    [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
                    [cell setAccessoryView:nil];
                    break;
            }
            break;
            
        case kWSSettingsSectionSensors:
            switch (indexPath.row) {
                case kWSSettingsSensorsShowSensorsRow:
                    [[cell textLabel] setText:kWSSettingsSensorsShowSensorsRowLabel];
                    [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
                    [cell setAccessoryView:nil];
                    break;
                case kWSSettingsSensorsShowPendingOperationsRow:
                    [[cell textLabel] setText:kWSSettingsSensorsShowPendingOperationsRowLabel];
                    [cell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
                    [cell setAccessoryView:nil];
                    break;
            }
            break;
    }
    
    return (cell);
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return (kWSSettingsSectionsCount);
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (section) {
        case kWSSettingsSectionLogging:
            return (kWSSettingsLoggingRowsCount);
        case kWSSettingsSectionSensors:
            return (kWSSettingsSensorsRowsCount);
        default:
            return (0);
    }
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    switch (section) {
        case kWSSettingsSectionLogging:
            return (NSLocalizedString(kWSSettingsSectionLoggingLabel, nil));
        case kWSSettingsSectionSensors:
            return (NSLocalizedString(kWSSettingsSectionSensorsLabel, nil));
        default:
            return (@"");
    }
}

#pragma mark - Interface events

- (IBAction)switchToggledForSwitch:(UISwitch *)sender
{
    NSString *key = nil;
    
    switch ([sender tag]) {
        case kWSSettingsLoggingTouchLoggingSwitchTag:
            key = kSettingsTouchLoggingEnabled;
            break;
        case kWSSettingsLoggingMotionLoggingSwitchTag:
            key = kSettingsMotionLoggingEnabled;
            break;
        case kWSSettingsLoggingNetworkLoggingSwitchTag:
            key = kSettingsNetworkLoggingEnabled;
            break;
        case kWSSettingsLoggingDeviceLoggingSwitchTag:
            key = kSettingsDeviceLoggingEnabled;
            break;
        case kWSSettingsLoggingShowLoggingPanelSwitchTag:
            key = kSettingsLoggingPanelEnabled;
            break;
        case kWSSettingsLoggingVerboseLoggingSwitchTag:
            key = kSettingsVerboseLoggingEnabled;
            break;
        default:
            return;
    }
    
    [[NSUserDefaults standardUserDefaults] setBool:[sender isOn] forKey:key];
}

@end
