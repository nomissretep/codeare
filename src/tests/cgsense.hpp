/*
 *  codeare Copyright (C) 2007-2010 Kaveh Vahedipour
 *                               Forschungszentrum Juelich, Germany
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 
 *  02110-1301  USA
 */

template <class T> bool 
cgsensetest (RRClient::Connector<T>* rc) {

	// Incoming
	Matrix<cxfl>   rawdata; // Measurement data O(Nkx,Nky,Nkz)
	Matrix<double>  weights; // NUFFT weights   
	Matrix<double>  kspace;  // Kspace positions O(Nkx,Nky,Nkz)
	Matrix<cxfl>   sens;    // Sensitivity maps O(Nx, Ny, Nz)
	
	// Outgoing
	Matrix<double>  nrmse;   // Residues of the CG process
	Matrix<cxfl>   image;
	Matrix<cxfl>   signals;
	
	std::string    cf  = std::string (base + std::string(config));        // Configuration file (f.e. share/cgsense/config_human.xml)
	std::string    df  = std::string (base + std::string(data));          // Binary data file 
	std::string    odf = std::string (base + std::string("/images.mat")); // Binary Ouput (images etc)
	
	Read (weights, df, "weights");
	Read (rawdata, df, "data");
	Read (kspace,  df, "kspace");
	Read (sens,    df, "sensitivities");
	
	rc->ReadConfig (cf.c_str());
	
	if (rc->Init (test) != OK) {
		printf ("Intialising failed ... bailing out!"); 
		return false;
	}
	
	// Prepare -------------
	
	rc->SetMatrix (   "sens", sens);    // Sensitivities
	rc->SetMatrix ("weights", weights); // Weights
	rc->SetMatrix ( "kspace", kspace);  // K-space
	
	rc->Prepare   (test);
	
	// Process -------------

	rc->SetMatrix (   "data", rawdata); // Measurement data
	
	rc->Process   (test);
	
	// Receive -------------
	
	rc->GetMatrix (  "image", image);  // Images
	rc->GetMatrix (  "nrmse", nrmse);  // CG residuals
	
	// ---------------------
	
	rc->Finalise   (test);
	
#ifdef HAVE_MAT_H	
	MATFile* mf = matOpen (odf.c_str(), "w");
	
	if (mf == NULL) {
		printf ("Error creating file %s\n", odf.c_str());
		return false;
	}
	
	MXDump       (image, mf, "image");
	if (pulses)
		MXDump (signals, mf, "signals");
	if (nrmse.Size() > 1)
		MXDump   (nrmse, mf, "nrmse");
	
	if (matClose(mf) != 0) {
		printf ("Error closing file %s\n", odf.c_str());
		return false;
	}
#endif

	return true;
	
}
