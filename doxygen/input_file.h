/**
 * \page input_file Input Specifications
 *
 * The input file should be a valid XML-file. This means each car must have a corresponding output tag. All cars and
 * roads must be specified inside the <ROOT> </ROOT> tags.
 *
 * \section Road
 *
 * A road must be specified between his own the <BAAN> </BAAN> tags. Each road must contain following elements:
 *  - naam: The name of the road (String)
 *  - snelheidslimiet: The speed limit of the road (Integer)
 *  - lengte: The length of the road (Integer)
 *
 *  \section Vehicle
 *
 *  A vehicle must be specified between his own <VOERTUIG> </VOERTUIG> tags. Each vehicle must contain following
 *  elements:
 *  - type: The type of the vehicle (currently is car (auto) the only supported type) (String)
 *  - nummerplaat: The license plate of the vehicle (String)
 *  - baan: The name of the current road the car is driving on (String)
 *  - positie: The position of the vehicle on the current road. (Integer)
 *  - snelheid: The current speed the car is driving at. (Integer)
 *
 *  \section Example
 *
 *  <ROOT>
 *     <BAAN>
 *         <naam>E19</naam>
 *         <snelheidslimiet>100</snelheidslimiet>
 *         <lengte>2000</lengte>
 *     </BAAN>
 *     <VOERTUIG>
 *         <type>AUTO</type>
 *         <nummerplaat>1THK180</nummerplaat>
 *         <baan>E19</baan>
 *         <positie>10</positie>
 *         <snelheid>0</snelheid>
 *     </VOERTUIG>
 *     <VOERTUIG>
 *         <type>AUTO</type>
 *         <nummerplaat>651BUF</nummerplaat>
 *         <baan>E19</baan>
 *         <positie>0</positie>
 *         <snelheid>0</snelheid>
 *     </VOERTUIG>
 * </ROOT>
 */

